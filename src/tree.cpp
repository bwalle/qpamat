/*
 * Id: $Id: tree.cpp,v 1.16 2003/12/16 12:30:24 bwalle Exp $
 * -------------------------------------------------------------------------------------------------
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; You may only use 
 * version 2 of the License, you have no option to use any other version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ------------------------------------------------------------------------------------------------- 
 */
#include <cstdlib>

#include <qstatusbar.h>
#include <qfile.h>
#include <qdom.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qheader.h>
#include <qiconset.h>
#include <qevent.h>
#include <qcursor.h>
#include <qprogressdialog.h>

#include "../images/edit_remove_16x16.xpm"
#include "../images/rename_16x16.xpm"
#include "../images/edit_add_16x16.xpm"

#include "qpamat.h"
#include "tree.h"
#include "treeentry.h"
#include "wrongpassword.h"
#include "security/passwordhash.h"
#include "security/stringencryptor.h"
#include "security/symmetricencryptor.h"
#include "security/collectencryptor.h"
#include "security/notencryptor.h"
#include "smartcard/memorycard.h"
#include "settings.h"

// -------------------------------------------------------------------------------------------------
Tree::Tree(QWidget* parent)
// -------------------------------------------------------------------------------------------------
    : QListView(parent)
{
    addColumn("first");
    header()->setStretchEnabled(true);
    header()->hide();
    setRootIsDecorated(true);
    setShowSortIndicator(true);
    
    setFocusPolicy(QWidget::StrongFocus);
    viewport()->setAcceptDrops(true);
    setAcceptDrops(true);
    
    initTreeContextMenu();
    connect(this, SIGNAL(contextMenuRequested(QListViewItem*, const QPoint&, int)),
        SLOT(showContextMenu(QListViewItem*, const QPoint&)));
    connect(this, SIGNAL(currentChanged(QListViewItem*)), 
        this, SLOT(currentChangedHandler(QListViewItem*)));
    connect(this, SIGNAL(dropped(QDropEvent*)), SLOT(droppedHandler(QDropEvent*)));
}


// -------------------------------------------------------------------------------------------------
void Tree::keyPressEvent(QKeyEvent* evt)
// -------------------------------------------------------------------------------------------------
{
    switch (evt->key())
    {
        case Key_Delete:
            deleteCurrent();
            break;
        default:
            evt->ignore();
            QListView::keyPressEvent(evt);
            break;
    }
}

// -------------------------------------------------------------------------------------------------
bool Tree::readFromXML(const QString& fileName, const QString& password) throw (WrongPassword)
// -------------------------------------------------------------------------------------------------
{
    // delete the old tree
    clear();
    
    QSettings& set = Settings::getInstance().getSettings();
    bool smartcard = set.readBoolEntry("Smartcard/UseCard", Settings::DEFAULT_USE_CARD );
    
    // load the XML structure
    QFile file(fileName);
    if (!file.open(IO_ReadOnly))
    {
        QMessageBox::critical(this, "QPaMaT", tr("The file %1 could not be opened:\n%2.").
            arg(fileName).arg(qApp->translate("QFile", file.errorString())), QMessageBox::Ok, 
            QMessageBox::NoButton);
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        showCurruptedMessage(fileName);
        return false;
    }
    file.close();
    QDomElement root = doc.documentElement();
    
    // check the password
    try
    {
        if (!PasswordHash::isCorrect(password, root.attribute("password-hash")))
        {
            throw WrongPassword(tr("The password is incorrect.").latin1());
        }
    }
    catch (const std::invalid_argument& ex)
    {
        qDebug(ex.what());
        showCurruptedMessage(fileName);
        return false;
    }
    
    if (root.attribute("card-id") && !smartcard)
    {
        QMessageBox::warning(this, "QPaMaT", tr("<qt><nobr>The passwords of the current data file"
            " are stored</nobr> on a smartcard but you did not configure QPaMaT for reading "
            "smartcards.<p>Change the settings and try again!</qt>"), QMessageBox::Ok,
            QMessageBox::NoButton);
        return false;
    }
    
    try
    {
        QString algorithm = root.attribute("crypt-algorithm");
        StringEncryptor* enc = 0;
        Encryptor* realEncryptor = 0;
        try
        {
            if (smartcard)
            {
                realEncryptor = new SymmetricEncryptor(algorithm, password);
                enc = new CollectEncryptor(*realEncryptor);
            }
            else
            {
                enc = new SymmetricEncryptor(algorithm, password);
            }
        }
        catch (const NoSuchAlgorithmException& ex)
        {
            QMessageBox::critical(this, "QPaMaT", tr("The algorithm '%1' is not avaible on "
                "your system.\nIt is impossible to read the file. Try to recompile or\n",
                "update your OpenSSL library.").arg(algorithm), QMessageBox::Ok, QMessageBox::NoButton);
            return false;
        }
        
        // read the data from the smartcard
        if (smartcard && root.attribute("card-id"))
        {
            bool success = false;
            ByteVector vec;
            while (!success)
            {
                byte id = byte(root.attribute("card-id").toShort());
                success = writeOrReadSmartcard(vec, false, id);
                
                if (!success && QMessageBox::question(this, "QPaMaT", tr("Reading from the "
                    "smartcard was not successful.\nDo you want to try again?"), QMessageBox::Yes |
                    QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
                {
                    delete enc;
                    delete realEncryptor;
                    return false;
                }
            }
            dynamic_cast<CollectEncryptor*>(enc)->setBytes(vec);
        }
        
        QDomNode n = root.firstChild();
        while( !n.isNull() ) 
        {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if( !e.isNull() ) 
            {
                TreeEntry::appendFromXML(this, e, *enc);
            }
            n = n.nextSibling();
        }
    
        delete enc;
        delete realEncryptor;
    }
    catch (const std::exception& e)
    {
        clear();
        showReadErrorMessage(e.what());
        return false;
    }
    
    qpamat->message(tr("Reading of data finished successfully."), false);
    
    return true;
}


// -------------------------------------------------------------------------------------------------
bool Tree::writeToXML(const QString& fileName, const QString& password, const QString& algorithm)
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    bool smartcard = set.readBoolEntry("Smartcard/UseCard", Settings::DEFAULT_USE_CARD );
    QFile file(fileName);
    if (!file.open(IO_WriteOnly))
    {
        QMessageBox::critical(this, "QPaMaT", tr("The data could not be saved. There "
            "was an\nerror while creating the file:\n%1").arg( qApp->translate("QFile",
            file.errorString())), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    QDomDocument doc;
    QDomElement root = doc.createElement("qpamat");
    root.setAttribute("version", VERSION);
    root.setAttribute("crypt-algorithm", algorithm);
    root.setAttribute("password-hash", PasswordHash::generateHash(password));
    doc.appendChild(root);
    
    StringEncryptor* enc;
    Encryptor* realEncryptor = 0;
    try
    {
        if (smartcard)
        {
            realEncryptor = new SymmetricEncryptor(algorithm, password);
            enc = new CollectEncryptor(*realEncryptor);
        }
        else
        {
            enc = new SymmetricEncryptor(algorithm, password);
        }
    }
    catch (const NoSuchAlgorithmException& e)
    {
        QMessageBox::critical(this, "QPaMaT", tr("The algorithm '%1' is not avaible on "
            "your system.\nChoose another crypto algorithm in the settings.\nThe data "
            "is not saved!").arg(algorithm), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    
    // we have one child that contains all children. The root child is not appended to
    // XML so we have to find the children of the child
    TreeEntry* currentItem = dynamic_cast<TreeEntry*>(firstChild());
    while (currentItem)
    {
        currentItem->appendXML(doc, root, *enc);
        currentItem = dynamic_cast<TreeEntry*>(currentItem->nextSibling());
    }
    
    bool success = false; 
    while (!success)
    {
        byte id = 0;
        if (smartcard)
        {
            ByteVector vec = dynamic_cast<CollectEncryptor*>(enc)->getBytes();
            success = writeOrReadSmartcard(vec, true, id);
            root.setAttribute("card-id", id);
        }
        else
        {
            success = true;
        }
        
        if (!success && QMessageBox::question(this, "QPaMaT", tr("Writing to the smartcard was not"
            " successful.\nDo you want to try again?"), QMessageBox::Yes | QMessageBox::Default, 
            QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
        {
            break;
        }
    }
    
    delete enc;
    delete realEncryptor;
    
    if (!success)
    {
        qpamat->message(tr("No data was saved!"));
        return false;
    }
    else
    {
        qpamat->message(tr("Data successfully written."), false);
    }
    
    QTextStream stream(&file);
    stream.setEncoding(QTextStream::UnicodeUTF8);
    stream << doc.toString();
    
    return true;
}


// -------------------------------------------------------------------------------------------------
QDragObject* Tree::dragObject()
// -------------------------------------------------------------------------------------------------
{
    QListViewItem* current = currentItem();
    emit stateModified();
    if (current)
    {
        QString xml = dynamic_cast<TreeEntry*>(current)->toXML();
        QStoredDrag* drag = new QStoredDrag("application/x-qpamat", this);
        drag->setEncodedData(xml.utf8());
        return drag;
    }
    return 0;
}


// -------------------------------------------------------------------------------------------------
void Tree::showCurruptedMessage(const QString& fileName)
// -------------------------------------------------------------------------------------------------
{
    QMessageBox::critical(this, "QPaMaT", tr("The XML file (%1) may be corrupted "
            "and\ncould not be read. Check the file with a text editor.").arg(fileName), 
            QMessageBox::Ok, QMessageBox::NoButton);
}


// -------------------------------------------------------------------------------------------------
void Tree::showReadErrorMessage(const QString& message)
// -------------------------------------------------------------------------------------------------
{
    QMessageBox::critical(this, "QPaMaT", tr("An unknown error occured while reading the "
            "file. If you don't know what to do try to contact the author.<p>The error message "
            "was:<br><nobr>%1</nobr>").arg(message), 
            QMessageBox::Ok, QMessageBox::NoButton);
}


// -------------------------------------------------------------------------------------------------
void Tree::showContextMenu(QListViewItem* item, const QPoint& point) 
// -------------------------------------------------------------------------------------------------
{
    m_contextMenu->setItemEnabled(DELETE_ITEM, item != 0);
    m_contextMenu->setItemEnabled(RENAME_ITEM, item != 0);
    int id = m_contextMenu->exec(point);
    
    switch (id)
    {
        case DELETE_ITEM:
            delete item;
            emit stateModified();
            break;
        case INSERT_ITEM:
            insertItem(dynamic_cast<TreeEntry*>(item), false);
            break;
        case INSERT_CATEGORY:
            insertItem(dynamic_cast<TreeEntry*>(item), true);
            break;
        case RENAME_ITEM:
            dynamic_cast<TreeEntry*>(item)->startRename(0);
            emit stateModified();
            break;
        case -1:
            break;
        default: 
            qDebug("Error in showContextMenu\n");
            break;
    }
}


// -------------------------------------------------------------------------------------------------
void Tree::insertItem(TreeEntry* item, bool category)
// -------------------------------------------------------------------------------------------------
{
    if (!hasFocus())
    {
        return;
    }
    
    const QString name = category
        ? tr("New category")
        : tr("New Item");
    QListViewItem* newItem = 0;
    if (item)
    {
        if (! item->isCategory())
        {
            if (currentItem()->parent())
            {
                newItem = new TreeEntry( item->QListViewItem::parent(), name, category);
            }
            else
            {
                newItem = new TreeEntry(this, name, category);
            }
        }
        else
        {
            newItem = new TreeEntry( item, name, category);
        }
    }
    else
    {
        newItem = new TreeEntry( this, name, category);
    }
    setSelected(newItem, true);
    newItem->startRename(0);
    emit stateModified();
}


// -------------------------------------------------------------------------------------------------
void Tree::deleteCurrent()
// -------------------------------------------------------------------------------------------------
{
    if (hasFocus())
    {
        QListViewItem* selected = selectedItem();
        if (selected)
        {
            delete selected;
            emit stateModified();
        }
        else
        {
            qpamat->message(tr("No item selected!"));
        }
    }
}


// -------------------------------------------------------------------------------------------------
void Tree::insertAtCurrentPos()
// -------------------------------------------------------------------------------------------------
{
    if (hasFocus())
    {
        insertItem(dynamic_cast<TreeEntry*>(currentItem()));
    }
}


// -------------------------------------------------------------------------------------------------
void Tree::initTreeContextMenu()
// -------------------------------------------------------------------------------------------------
{
    m_contextMenu = new QPopupMenu(this);
    m_contextMenu->insertItem(QIconSet(edit_add_16x16_xpm), 
        tr("Insert &Item") + "\t" + QString(QKeySequence(Key_Insert)), INSERT_ITEM);
    m_contextMenu->insertItem(tr("Insert &Category"), INSERT_CATEGORY);
    
    m_contextMenu->insertSeparator();
    
    m_contextMenu->insertItem(QIconSet(rename_16x16_xpm), 
        tr("&Rename") + "\t" + QString(QKeySequence(Key_F2)), RENAME_ITEM);
    m_contextMenu->insertItem(QIconSet(edit_remove_16x16_xpm), 
        tr("Delete &Item") + "\t" + QString(QKeySequence(Key_Delete)), DELETE_ITEM);
    
}

// -------------------------------------------------------------------------------------------------
QString Tree::toRichTextForPrint() 
// -------------------------------------------------------------------------------------------------
{
    QListViewItemIterator it(this);
    QListViewItem* current;
    QString ret;
    ret += "<qt>";
    while ( (current = it.current()) ) 
    {
        ret += dynamic_cast<TreeEntry*>(current)->toRichTextForPrint();
        ++it;
    }
    ret += "</qt>";
    return ret;
}


// -------------------------------------------------------------------------------------------------
void Tree::searchFor(const QString& word)
// -------------------------------------------------------------------------------------------------
{
    QListViewItem* selected = selectedItem();
    QListViewItem* current;
   
    QListViewItemIterator* it;
    if (selected)
    {
        it = new QListViewItemIterator(selected);
        ++(*it);
    }
    else
    {
        it = new QListViewItemIterator(this);
    }
    
    while ( (current = it->current()) ) 
    {
        if (current->text(0).contains(word, false))
        {
            setSelected(current, true);
            // open all items
            QListViewItem* loopItem = current;
            while (loopItem->parent())
            {
                loopItem = loopItem->parent();
                loopItem->setOpen(true);
            }
            break;
        }
        ++(*it);
    }
    
    delete it;
    
    if (selectedItem() == selected)
    {
        qpamat->message("No items found");
    }
}


// -------------------------------------------------------------------------------------------------
void Tree::currentChangedHandler(QListViewItem*)
// -------------------------------------------------------------------------------------------------
{
    if (selectedItem() == 0)
    {
        emit selectionCleared();
        //qpamat->getRemoveItemAction()->setEnabled(false);
    }
}


// -------------------------------------------------------------------------------------------------
bool Tree::writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber)
// -------------------------------------------------------------------------------------------------
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    
    // at first we need a random number 
    if (write)
    {
        std::srand(std::time(0));
        randomNumber = byte((double(std::rand())/RAND_MAX)*256);
    }
    
    QSettings& set = Settings::getInstance().getSettings();
    QString library = set.readEntry("Smartcard/Library", "" );
    int port = set.readNumEntry("Smartcard/Port", Settings::DEFAULT_PORT);
    
    try
    {
        MemoryCard card(library);
        
        card.init(port);
        
        QApplication::restoreOverrideCursor();
        
        QMessageBox::information(this, "QPaMaT", tr("Insert the smartcard in your reader!"), 
            QMessageBox::Ok, QMessageBox::NoButton);
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
        
        if (card.getType() != MemoryCard::TMemoryCard)
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "QPaMaT", tr("There's no memory card in your "
                "reader.\nUse the test function in the configuration\ndialog to set up your "
                "reader properly."), QMessageBox::Ok, QMessageBox::NoButton);
            return false;
        }
        
        if (!card.selectFile())
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "QPaMaT", "<qt>" + tr("It was not possible to select the "
                "file on the smartcard") + "</qt>", QMessageBox::Ok, QMessageBox::NoButton);
            return false;
        }
        
        if (write)
        {
            // write the random number
            ByteVector byteVector(1);
            byteVector[0] = randomNumber;
            card.write(0, byteVector);
            
            qDebug("Random = %d", byteVector[0]);
            
            // then write the number of bytes
            int numberOfBytes = bytes.size();
            byteVector.resize(3);
            byteVector[0] = (numberOfBytes & 0xFF00) >> 8;
            byteVector[1] = numberOfBytes & 0xFF;
            byteVector[2] = 0; // fillbyte
            card.write(1, byteVector);
            
            qDebug("First = %d\nSecond = %d", byteVector[0], byteVector[1]);
            
            // and finally write the data
            card.write(4, bytes);
        }
        else
        {
            // read the random number
            if (card.read(0, 1)[0] != randomNumber)
            {
                QApplication::restoreOverrideCursor();
                QMessageBox::critical(this, "QPaMaT", tr("You inserted the wrong smartcard!"), 
                    QMessageBox::Ok, QMessageBox::NoButton);
                return false;
            }
            
#ifdef DEBUG
            qDebug("Reading smartcard.");
            qDebug("Read randomNumber = %d", randomNumber);
#endif

            // read the number
            ByteVector vec = card.read(1, 2);
            int numberOfBytes = (vec[0] << 8) + (vec[1]);
            
#ifdef DEBUG
            qDebug("Read numberOfBytes = %d", numberOfBytes);
#endif

            Q_ASSERT(numberOfBytes >= 0);
            
            // read the bytes
            bytes = card.read(4, numberOfBytes);
        }
        card.close(); // if not, the destructor does, no problem for exceptions here!
    }
    catch (const NoSuchLibraryException& e)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", tr("The application was not set up correctly "
            "for using the smartcard. Call the configuration dialog and use the Test button "
            "for testing!<p>The error message was:<br><nobr>%1</nobr>").arg(e.what()), 
            QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    catch (const CardException& e)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", tr("There was a communication error while "
            "writing to the card.<p>The error message was:<br><nobr>%1</nobr>").arg(e.what()), 
            QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    
    QApplication::restoreOverrideCursor();
    return true;
}


// -------------------------------------------------------------------------------------------------
void Tree::droppedHandler(QDropEvent* evt)
// -------------------------------------------------------------------------------------------------
{
    if (evt->provides("application/x-qpamat"))
    {
        evt->accept();
        QString xml = QString::fromUtf8(evt->encodedData("application/x-qpamat"));
        QDomDocument doc;
        doc.setContent(xml);
        QDomElement elem = doc.documentElement();
        QListViewItem* src = reinterpret_cast<TreeEntry*>(elem.attribute("memoryAddress").toLong());
        NotEncryptor enc;
        QListViewItem* appended = TreeEntry::appendFromXML(this, elem, enc);
        setSelected(appended, true);
        delete src;
    }
}

