/*
 * Id: $Id: tree.cpp,v 1.23 2003/12/30 19:45:31 bwalle Exp $
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
#include <qthread.h>
#include <qdom.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qheader.h>
#include <qiconset.h>
#include <qevent.h>
#include <qcursor.h>
#include <qeventloop.h>
#include <qfileinfo.h>
#include <qprogressdialog.h>

#include "images/edit_remove_16x16.xpm"
#include "images/rename_16x16.xpm"
#include "images/edit_add_16x16.xpm"
#include "images/traffic_yellow_16x16.xpm"
#include "images/traffic_red_16x16.xpm"
#include "images/traffic_green_16x16.xpm"
#include "images/traffic_gray_16x16.xpm"
#include "images/smartcard_24x24.xpm"

#include "qpamat.h"
#include "tree.h"
#include "treeentry.h"
#include "security/passwordhash.h"
#include "security/stringencryptor.h"
#include "security/symmetricencryptor.h"
#include "security/collectencryptor.h"
#include "security/notencryptor.h"
#include "dialogs/waitdialog.h"
#include "smartcard/memorycard.h"
#include "settings.h"


/*!
    \class Tree
    
    \brief Represents the tree that holds the password entries.

    This class also handles reading and writing to XML files (by calling the right methods
    of an TreeEntry) and -- very important -- writing to the smartcard. Each XML file has a
    random number which is created on each successful write. It is used to identify the card.
    
    So the first byte (a value between 0 and 255) is the random number. Then two bytes on the
    card indicate the number of bytes stored. Then a fill byte is stored which is reserved for future
    use. The 5th byte on the card is then the first real byte for passwords.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.23 $
    \date $Date: 2003/12/30 19:45:31 $
*/

/*!
    \enum Tree::MenuID
    
    Holds the menu ids (context menu)
*/

/*!
    \fn Tree::selectionCleared()
    
    If the tree has a current item but no item is selected.
*/

/*!
    \fn Tree::stateModified()
    
    If something was modified, need to determine if saving is necessary.
*/

/*!
    Creates a new instance of a Tree.
    \param parent the parent
*/
Tree::Tree(QWidget* parent)
    : QListView(parent), m_showPasswordStrength(false)
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


/*!
    Initializes the context menu
*/
void Tree::initTreeContextMenu()
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


/*!
    If the users presses a key.
    \param evt the key event
*/
void Tree::keyPressEvent(QKeyEvent* evt)
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


/*!
    Reads and updates the tree from the given XML file.
    \param fileName the XML file from which the information should be read
    \param password the password
    \return \c true on success, \c false  otherwise
    \exception WrongPassword if the given password was wrong according to the XML file
*/
bool Tree::readFromXML(const QString& fileName, const QString& password) throw (WrongPassword)
{
    // delete the old tree
    if (childCount() > 0)
    {
        clear();
    }
    
    bool smartcard = qpamat->set().readBoolEntry("Smartcard/UseCard");
    
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
    smartcard = !root.attribute("card-id").isNull();
    
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


/*!
    Writs the current tree to the XML file. The error handling is done by the function,
    i.e. suitable dialog messages are displayed.
    \param fileName the file name where the tree should be written
    \param password the password for encryption
    \param algorithm the algorithm for encryption
    \return \c true on success, \c false  otherwise
*/
bool Tree::writeToXML(const QString& fileName, const QString& password, const QString& algorithm)
{
    bool smartcard = qpamat->set().readBoolEntry("Smartcard/UseCard");
    QFile file(fileName);
    if (!QFileInfo(file).isWritable())
    {
        QMessageBox::critical(this, "QPaMaT", tr("<qt><nobr>The data file is not writable. Change "
            "the file in</nobr> the configuration dialog or change the permission of the file!"
            "</qt>"), QMessageBox::Ok, QMessageBox::NoButton);
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
    
    if (!file.open(IO_WriteOnly))
    {
        QMessageBox::critical(this, "QPaMaT", tr("The data could not be saved. There "
            "was an\nerror while creating the file:\n%1").arg( qApp->translate("QFile",
            file.errorString())), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    
    QTextStream stream(&file);
    stream.setEncoding(QTextStream::UnicodeUTF8);
    stream << doc.toString();
    
    return true;
}


/*!
    Returns the appropriage QDragObject for performing Drag and drop
    \return the object
*/
QDragObject* Tree::dragObject()
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


/*!
    Helping function to display that a \p fileName is corrupted. Displays a message box.
    \param fileName the file name that is included in the message
*/
void Tree::showCurruptedMessage(const QString& fileName)
{
    QMessageBox::critical(this, "QPaMaT", tr("The XML file (%1) may be corrupted "
            "and\ncould not be read. Check the file with a text editor.").arg(fileName), 
            QMessageBox::Ok, QMessageBox::NoButton);
}


/*!
    Helping function that shows a message box that indicates that a read error has occured
    \param message the real error message (from the smart card classes)
*/
void Tree::showReadErrorMessage(const QString& message)
{
    QMessageBox::critical(this, "QPaMaT", tr("An unknown error occured while reading the "
            "file. If you don't know what to do try to contact the author.<p>The error message "
            "was:<br><nobr>%1</nobr>").arg(message), 
            QMessageBox::Ok, QMessageBox::NoButton);
}


/*!
    Displays the context menu. Should be connected to the contextMenuRequested() signal of the
    Tree.
    \param item the menu item 
    \param point the coordinates of the click
*/
void Tree::showContextMenu(QListViewItem* item, const QPoint& point) 
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


/*!
    Inserts a item at the specified position
    \param item the item
    \param category if the new item should be a category
*/
void Tree::insertItem(TreeEntry* item, bool category)
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


/*!
    Deletes the currently \b selected entry
*/
void Tree::deleteCurrent()
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


/*!
    Inserts an item at the current position
*/
void Tree::insertAtCurrentPos()
{
    if (hasFocus())
    {
        insertItem(dynamic_cast<TreeEntry*>(currentItem()));
    }
}


/*!
    Converts the tree to Rich text (HTML). This function is for printing.
    \return the text
*/
QString Tree::toRichTextForPrint() 
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


/*!
    Performs a search operation.
    \param word the word to search for (case insensitive)
*/
void Tree::searchFor(const QString& word)
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
            ensureItemVisible(current);
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


/*!
    Handler that is called if the current item has changed (usually after delete operations). It
    Must change the selection on the right panel.
*/
void Tree::currentChangedHandler(QListViewItem*)
{
    if (selectedItem() == 0)
    {
        emit selectionCleared();
    }
}


/*!
    Handler for items that are dropped directly to the white space at the list view.
    \param evt the event
*/
void Tree::droppedHandler(QDropEvent* evt)
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
        updatePasswordStrengthView(); 
    }
}


/*!
    Recomputes the password strength. This function should be called before showing the password
    strength in the tree and it must be called after changing the configuration related to
    password strength. It shows a progress dialog.
    \param error is set to \c true if an error occured and if \p error is not \c NULL
*/
void Tree::recomputePasswordStrength(bool* error)
{
    if (!m_showPasswordStrength)
    {
        return;
    }
    
    if (error)
    {
        *error = true;
    }
    
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    int num = 0;
    {
        QListViewItemIterator it(this);
        while (it.current()) 
        {
            TreeEntry* current = dynamic_cast<TreeEntry*>(it.current());
            TreeEntry::PropertyIterator propIt = current->propertyIterator();
            while (propIt.current())
            {
                if (propIt.current()->getType() == Property::PASSWORD)
                {
                    ++num;
                }
                ++propIt;
            }
            ++it;
        }
    }
    
    try
    {
        QProgressDialog progress( tr("Updating password strength..."), 0, num, this, "progress", false);
        progress.setMinimumDuration(200);
        progress.setCaption("QPaMaT");
        int progr = 0;
    
        QListViewItemIterator it(this);
        while (it.current()) 
        {
            TreeEntry* current = dynamic_cast<TreeEntry*>(it.current());
            TreeEntry::PropertyIterator propIt = current->propertyIterator();
            while (propIt.current())
            {
                if (propIt.current()->getType() == Property::PASSWORD)
                {
                    propIt.current()->updatePasswordStrength();
                    progress.setProgress(progr++);
                    qDebug("progr = %d", progr);
                    qApp->processEvents();
                }
                ++propIt;
            }
            ++it;
        }
        
        progress.setProgress(num);
        qApp->processEvents();
        QApplication::restoreOverrideCursor();
        
        updatePasswordStrengthView();
        
        if (error)
        {
            *error = false;
        }
    }
    catch (const PasswordCheckException& e)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, "QPaMaT", tr("<qt><nobr>Failed to calculate the password "
                "strength. The error message</nobr> was:<p>%1<p>Check your configuration!</qt>")
                .arg(e.what()), QMessageBox::Ok, QMessageBox::NoButton);
    }
}


/*!
    Shows the icons that indicate weak passwords on the left.
    \param enabled if \p show is \c true, the markers are shown, otherwise they get hidden
*/
void Tree::setShowPasswordStrength(bool enabled)
{
    m_showPasswordStrength = enabled;
}


/*!
    Updates the icons based on the last settings of setShowPasswordStrength()
*/
void Tree::updatePasswordStrengthView()
{
    try
    {
        if (m_showPasswordStrength)
        {
            QListViewItemIterator it(this);
            while (it.current()) 
            {
                Property::PasswordStrength strength = 
                    dynamic_cast<TreeEntry*>(it.current())->weakestChildrenPassword();
                switch (strength)
                {
                    case Property::PWeak:
                        it.current()->setPixmap(0, traffic_red_16x16_xpm);
                        break;
                    case Property::PAcceptable:
                        it.current()->setPixmap(0, traffic_yellow_16x16_xpm);
                        break;
                    case Property::PStrong:
                        it.current()->setPixmap(0, traffic_green_16x16_xpm);
                        break;
                    case Property::PUndefined:
                        it.current()->setPixmap(0, traffic_gray_16x16_xpm);
                        break;
                    default:
                        qDebug("Tree::showWeakPasswordMarkers out of range");
                        break;
                }
                ++it;
            }
        }
        else
        {
            QListViewItemIterator it(this);
            while (it.current()) 
            {
                it.current()->setPixmap(0, 0);
                ++it;
            }
        }
    }
    catch (const PasswordCheckException& e)
    {
        QMessageBox::warning(this, "QPaMaT", tr("<qt><nobr>Failed to calculate the password "
            "strength. The error message</nobr> was:<p>%1<p>Check your configuration!</qt>")
            .arg(e.what()), QMessageBox::Ok, QMessageBox::NoButton);
    }
}


// -------------------------------------------------------------------------------------------------

class ReadWriteThread : public QThread
{
    public:
        ReadWriteThread(MemoryCard& card, ByteVector& bytes, bool write, byte& randomNumber);
        bool wasOk() const;
        QString errorText() const;
    protected:
        void run();
    private:
        MemoryCard& m_card;
        ByteVector& m_bytes;
        const bool m_write;
        byte& m_randomNumber;
        QString m_error;
};

// -------------------------------------------------------------------------------------------------


/*!
    \class ReadWriteThread
    
    Thread that is responsible for reading and writing to the smart card. Because the real
    operations are long and atomar, the GUI would be blocked if the operations are not running
    in an own thread.
    
    No GUI operations take place in this thread. Instead of that, if an error occured the
    error message is set and the operation is finished. The caller has to check the error message
    and must display a message.
    
    The access to the variables which are passed to the constructor are not locked with a
    QMutex or something like that. The caller must ensure that he doesn't access this variables
    while this thread is running. This is usually no problem because the caller displays just
    a dialog which says the user that he must wait until the operation is finished.
*/

/*!
    Creates a new instance of a ReadWriteThread.
    \param card the memory card, it must be initialized with the right port (the reason is that
           the user should get a message box, insert the card and confirm the box while the
           memory card class should wait for it. So another process cannot access the card terminal
           at this time, this is important for security reasons
    \param bytes the read or write bytes
    \param write \c true if a write operation should be made, \c false for a read operation
    \param rand the random number
*/
ReadWriteThread::ReadWriteThread( MemoryCard& card, ByteVector& bytes, bool write, byte& rand)
    :  m_card(card), m_bytes(bytes), m_write(write), m_randomNumber(rand)
{}


/*!
    Runs the operation.
*/
void ReadWriteThread::run()
{
    try
    {
        if (m_card.getType() != MemoryCard::TMemoryCard)
        {
            m_error = QObject::tr("There's no memory card in your reader.\nUse the test function in the "
                "configuration\ndialog to set up your reader properly.");
            return;
        }
        
        if (!m_card.selectFile())
        {
            m_error = "<qt>" + QObject::tr("It was not possible to select the file on "
                "the smartcard") + "</qt>";
            return;
        }
        
        if (m_write)
        {
            // write the random number
            ByteVector byteVector(1);
            byteVector[0] = m_randomNumber;
            m_card.write(0, byteVector);
            
#ifdef DEBUG
            qDebug("Random = %d", byteVector[0]);
#endif
            
            // then write the number of bytes
            int numberOfBytes = m_bytes.size();
            byteVector.resize(3);
            byteVector[0] = (numberOfBytes & 0xFF00) >> 8;
            byteVector[1] = numberOfBytes & 0xFF;
            byteVector[2] = 0; // fillbyte
            m_card.write(1, byteVector);
            
            // and finally write the data
            m_card.write(4, m_bytes);
            
        }
        else
        {
            // read the random number
            if (m_card.read(0, 1)[0] != m_randomNumber)
            {
                m_error = QObject::tr("You inserted the wrong smartcard!");
                return;
            }
            
#ifdef DEBUG
            qDebug("Reading smartcard.");
            qDebug("Read randomNumber = %d", m_randomNumber);
#endif

            // read the number
            ByteVector vec = m_card.read(1, 2);
            int numberOfBytes = (vec[0] << 8) + (vec[1]);
            
#ifdef DEBUG
            qDebug("Read numberOfBytes = %d", numberOfBytes);
#endif

            Q_ASSERT(numberOfBytes >= 0);
            
            // read the bytes
            m_bytes = m_card.read(4, numberOfBytes);
        }
    }
    catch (const CardException& e)
    {
        m_error = QObject::tr("There was a communication error while writing to the card.<p>The error "
            "message was:<br><nobr>%1</nobr>").arg(e.what());
        return;
    }
}


/*!
    Checks if running was Ok. This function should be called if the thread is not running
    any more.
    \return the bool value
*/
bool ReadWriteThread::wasOk() const
{
    return m_error.isNull();
}


/*!
    Returns the error text.
    \return hhe error text or QString::null if no error occured.
*/
QString ReadWriteThread::errorText() const
{
    return m_error;
}

// -------------------------------------------------------------------------------------------------

/*!
    Reads or writes from the smartcard. Displays an error message if needed. Refer to the
    ReadWriteThread for more information.
    \param bytes the bytes
    \param write reading or writing
    \param randomNumber the random number
*/
bool Tree::writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber)
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    
    // at first we need a random number 
    if (write)
    {
        std::srand(std::time(0));
        randomNumber = byte((double(std::rand())/RAND_MAX)*256);
    }
    
    QString library = qpamat->set().readEntry("Smartcard/Library");
    int port = qpamat->set().readNumEntry("Smartcard/Port");
    WaitDialog* msg = 0;
    
    try
    {
        // init the card
        MemoryCard card(library);
        card.init(port);
        
        // ask the user to insert the smartcard
        QApplication::restoreOverrideCursor();
        QMessageBox::information(this, "QPaMaT", tr("Insert the smartcard in your reader!"), 
            QMessageBox::Ok, QMessageBox::NoButton); 
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
        
        // start the thread
        ReadWriteThread thread(card, bytes, write, randomNumber);
        thread.start();
        
        // show dialog
        QString dlgText = write 
            ? tr("<b>Writing</b> to the smartcard...") 
            : tr("<b>Reading</b> from the smartcard..."); 
        msg = new WaitDialog(QPixmap(smartcard_24x24_xpm), dlgText, "QPaMaT", this, "Wait dialog");
        msg->show();
        
        // loop while the thread has finished
        QTimer timer;
        timer.start(100, 0);
        while (thread.running())
        {
            qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::WaitForMore);
        }
        timer.stop();
        
        // hide the dialog
        delete msg;
        QApplication::restoreOverrideCursor();
        qApp->processEvents();
        
        // error handling
        if (!thread.wasOk())
        {
            QMessageBox::critical(this, "QPaMaT", thread.errorText(), QMessageBox::Ok, 
                QMessageBox::NoButton);
            return false;
        }
    }
    catch (const NoSuchLibraryException& e)
    {
        delete msg;
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", tr("The application was not set up correctly for "
            "using the smartcard. Call the configuration dialog and use the Test button for "
            "testing!<p>The error message was:<br><nobr>%1</nobr>").arg(e.what()), 
            QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    
    return true;
}

// -------------------------------------------------------------------------------------------------

/*!
    \class WrongPassword
    
    \brief Exception that is thrown if the password is wrong.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.23 $
    \date $Date: 2003/12/30 19:45:31 $
*/

/*!
    \fn WrongPassword::WrongPassword(const std::string&)
    
    Creates a new instance of the exception and includes the error message. This
    message is returned by the what() method.
    \param error the error message
*/
