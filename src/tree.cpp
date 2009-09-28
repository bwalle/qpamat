/*
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

#include <boost/cast.hpp>

#include <QStatusBar>
#include <QFile>
#include <QThread>
#include <QDomDocument>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <Q3Header>
#include <QIcon>
#include <QEvent>
#include <QCursor>
#include <QEventLoop>
#include <QFileInfo>
#include <Q3ProgressDialog>
#include <QPixmap>
#include <QTextStream>
#include <QKeyEvent>
#include <QDropEvent>
#include <Q3PopupMenu>
#include <QDateTime>
#include <QDebug>

#include "qpamatwindow.h"
#include "qpamat.h"
#include "tree.h"
#include "treeentry.h"
#include "security/passwordhash.h"
#include "security/encryptor.h"
#include "security/symmetricencryptor.h"
#include "security/collectencryptor.h"
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
    : Q3ListView(parent)
    , m_showPasswordStrength(false)
{
    addColumn("first");
    header()->setStretchEnabled(true);
    header()->hide();
    setRootIsDecorated(true);
    setShowSortIndicator(true);

    setFocusPolicy(Qt::StrongFocus);
    viewport()->setAcceptDrops(true);
    setAcceptDrops(true);

    initTreeContextMenu();
    connect(this, SIGNAL(contextMenuRequested(Q3ListViewItem*, const QPoint&, int)),
        SLOT(showContextMenu(Q3ListViewItem*, const QPoint&)));
    connect(this, SIGNAL(currentChanged(Q3ListViewItem*)),
        this, SLOT(currentChangedHandler(Q3ListViewItem*)));
    connect(this, SIGNAL(dropped(QDropEvent*)), SLOT(droppedHandler(QDropEvent*)));
}


/*!
    Initializes the context menu
*/
void Tree::initTreeContextMenu()
{
    m_contextMenu = new Q3PopupMenu(this);
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/stock_add_16.png")),
        tr("Insert &Item") + "\t" + QString(QKeySequence(Qt::Key_Insert)), INSERT_ITEM);
    m_contextMenu->insertItem(tr("Insert &Category"), INSERT_CATEGORY);

    m_contextMenu->insertSeparator();

    m_contextMenu->insertItem(QIcon(QPixmap(":/images/rename_16.png")),
        tr("&Rename") + "\t" + QString(QKeySequence(Qt::Key_F2)), RENAME_ITEM);
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/stock_remove_16.png")),
        tr("Delete &Item") + "\t" + QString(QKeySequence(Qt::Key_Delete)), DELETE_ITEM);

}


/*!
    If the users presses a key.
    \param evt the key event
*/
void Tree::keyPressEvent(QKeyEvent* evt)
{
    switch (evt->key()) {
        case Qt::Key_Delete:
            deleteCurrent();
            break;

        default:
            evt->ignore();
            Q3ListView::keyPressEvent(evt);
            break;
    }
}


/*!
    Reads and updates the tree from the given XML file.
    \param rootElement the XML element which represents the <tt>\<qpamat\></tt> tag
*/
void Tree::readFromXML(const QDomElement& rootElement)
{
    // delete the old tree
    if (childCount() > 0)
        clear();

    QDomNode n = rootElement.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull())
            TreeEntry::appendFromXML(this, e);
        n = n.nextSibling();
    }

    // qpamat->message(tr("Reading of data finished successfully."), false);
}


/*!
    Appends the tree data to the specified QDomDocument. The document must contain a
    <tt>\<qpamat\></tt> document element which must contain a <tt>\<passwords\></tt> child.
    \param doc the QDomDocument to which the tree is appended. If the tree is empty, nothing is
               appended
    \exception std::invalid_argument if the given document does not met the described requirements
*/
void Tree::appendXML(QDomDocument& doc) const throw (std::invalid_argument)
{
    QDomElement docElem = doc.documentElement();
    QDomNode passwords = docElem.namedItem("passwords");
    if (passwords.isNull())
        throw std::invalid_argument("The QDomDocument must have a passwords tag as child of the "
            "qpamat document element.");

    // we have one child that contains all children. The root child is not appended to
    // XML so we have to find the children of the child
    TreeEntry* currentItem = dynamic_cast<TreeEntry*>(firstChild());
    while (currentItem) {
        currentItem->appendXML(doc, passwords);
        currentItem = dynamic_cast<TreeEntry*>(currentItem->nextSibling());
    }
}


/*!
    Returns the appropriage QDragObject for performing Drag and drop
    \return the object
*/
Q3DragObject* Tree::dragObject()
{
    Q3ListViewItem* current = currentItem();
    emit stateModified();
    if (current) {
        QString xml = boost::polymorphic_cast<TreeEntry*>(current)->toXML();
        Q3StoredDrag* drag = new Q3StoredDrag("application/x-qpamat", this);
        drag->setEncodedData(xml.utf8());
        return drag;
    }
    return 0;
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
void Tree::showContextMenu(Q3ListViewItem* item, const QPoint& point)
{
    if (!isEnabled())
        return;

    m_contextMenu->setItemEnabled(DELETE_ITEM, item != 0);
    m_contextMenu->setItemEnabled(RENAME_ITEM, item != 0);
    int id = m_contextMenu->exec(point);

    switch (id) {
        case DELETE_ITEM:
            deleteCurrent();
            break;

        case INSERT_ITEM:
            insertItem(dynamic_cast<TreeEntry*>(item), false);
            break;

        case INSERT_CATEGORY:
            insertItem(dynamic_cast<TreeEntry*>(item), true);
            break;

        case RENAME_ITEM:
            boost::polymorphic_cast<TreeEntry*>(item)->startRename(0);
            emit stateModified();
            break;

        case -1:
            break;

        default:
            qDebug() << CURRENT_FUNCTION << "Error in showContextMenu";
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
        return;

    const QString name = category
        ? tr("New category")
        : tr("New Item");
    Q3ListViewItem* newItem = 0;
    if (item) {
        if (!item->isCategory()) {
            if (currentItem()->parent())
                newItem = new TreeEntry( item->Q3ListViewItem::parent(), name, category);
            else
                newItem = new TreeEntry(this, name, category);
        }
        else
            newItem = new TreeEntry( item, name, category);
    } else
        newItem = new TreeEntry( this, name, category);
    setSelected(newItem, true);
    newItem->startRename(0);
    emit stateModified();
}


/*!
    Deletes the currently \b selected entry
*/
void Tree::deleteCurrent()
{
    if (hasFocus()) {
        Q3ListViewItem* selected = selectedItem();
        if (selected) {
            setOpen(selected, false);
            Q3ListViewItem* below = selected->itemBelow();
            if (!below || below->parent() != selected->parent()) {
                below = selected->itemAbove();
                if (!below || below->parent() != selected->parent())
                    below = selected->parent();
            }

            if (!below)
                below = lastItem();

            // check if the parent of the item that should made visible is deleted
            Q3ListViewItem* p = below;
            do {
                if (p == selected) {
                    below = 0;
                    break;
                }
            } while ( (p = p->parent()) );

            delete selected;

            if (below) {
                qDebug() << CURRENT_FUNCTION << "setSelected:" << below->text(0);
                setSelected(below, true);
            }
            emit stateModified();
        } else {
            QpamatWindow *win = Qpamat::instance()->getWindow();
            win->message(tr("No item selected!"));
        }
    }
}


/*!
    Inserts an item at the current position
*/
void Tree::insertAtCurrentPos()
{
    if (hasFocus())
        insertItem(dynamic_cast<TreeEntry*>(currentItem()));
}


/*!
    Converts the tree to Rich text (HTML). This function is for printing.
    \return the text
*/
QString Tree::toRichTextForPrint()
{
    Q3ListViewItemIterator it(this);
    Q3ListViewItem* current;
    QString ret;
    ret += "<qt>";
    while ( (current = it.current()) ) {
        ret += boost::polymorphic_cast<TreeEntry*>(current)->toRichTextForPrint();
        ++it;
    }
    ret += "</qt>";
    return ret;
}


/*!
    Appends the tree as text representation to the given stream. The text is formatted for
    export.

    \param stream the stream where the text is appended
*/
void Tree::appendTextForExport(QTextStream& stream)
{
    Q3ListViewItemIterator it(this);
    Q3ListViewItem* current;

    stream.setf(QTextStream::left);
    stream << qSetFieldWidth(20) << tr("QPaMaT") << qSetFieldWidth(0)
           << tr("password managing tool for Unix, Windows and MacOS X")  << "\n";
    stream << qSetFieldWidth(20) << tr("Export date:") << qSetFieldWidth(0)
           << QDateTime::currentDateTime().date().toString(Qt::ISODate) << "\n";
    stream << "================================================================================\n";

    while ( (current = it.current()) ) {
        boost::polymorphic_cast<TreeEntry*>(current)->appendTextForExport(stream);
        ++it;
    }
}


/*!
    Performs a search operation.
    \param word the word to search for (case insensitive)
*/
void Tree::searchFor(const QString& word)
{
    Q3ListViewItem* selected = selectedItem();
    Q3ListViewItem* current;

    Q3ListViewItemIterator* it;
    if (selected) {
        it = new Q3ListViewItemIterator(selected);
        ++(*it);
    } else
        it = new Q3ListViewItemIterator(this);

    while ( (current = it->current()) ) {
        if (current->text(0).contains(word, false)) {
            setSelected(current, true);
            ensureItemVisible(current);
            break;
        }
        ++(*it);
    }

    delete it;

    // wrap
    if (selectedItem() == selected) {
        it = new Q3ListViewItemIterator(this);
        while ( (current = it->current()) && current != selected ) {
            if (current->text(0).contains(word, false)) {
                setSelected(current, true);
                ensureItemVisible(current);
                break;
            }
            ++(*it);
        }
    }

    QpamatWindow *win = Qpamat::instance()->getWindow();
    if (selectedItem() == selected) {
        win->message(tr("No items found"));
    }
}


/*!
    Handler that is called if the current item has changed (usually after delete operations). It
    Must change the selection on the right panel.
*/
void Tree::currentChangedHandler(Q3ListViewItem*)
{
    if (selectedItem() == 0)
        emit selectionCleared();
}


/*!
    Handler for items that are dropped directly to the white space at the list view.
    \param evt the event
*/
void Tree::droppedHandler(QDropEvent* evt)
{
    if (evt->provides("application/x-qpamat")) {
        evt->accept();
        QString xml = QString::fromUtf8(evt->encodedData("application/x-qpamat"));
        QDomDocument doc;
        doc.setContent(xml);
        QDomElement elem = doc.documentElement();
        Q3ListViewItem* src = reinterpret_cast<TreeEntry*>(elem.attribute("memoryAddress").toLong());
        Q3ListViewItem* appended = TreeEntry::appendFromXML(this, elem);
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
        return;

    if (error)
        *error = true;

    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    int num = 0;

    {
        Q3ListViewItemIterator it(this);
        while (it.current()) {
            TreeEntry* current = boost::polymorphic_cast<TreeEntry*>(it.current());
            TreeEntry::PropertyIterator propIt = current->propertyIterator();
            while (propIt.current()) {
                if (propIt.current()->getType() == Property::PASSWORD)
                    ++num;
                ++propIt;
            }
            ++it;
        }
    }

    try {
        Q3ProgressDialog progress( tr("Updating password strength..."), 0, num, this, "progress",
                false);
        progress.setMinimumDuration(200);
        progress.setCaption("QPaMaT");
        int progr = 0;

        Q3ListViewItemIterator it(this);
        while (it.current()) {
            TreeEntry* current = boost::polymorphic_cast<TreeEntry*>(it.current());
            TreeEntry::PropertyIterator propIt = current->propertyIterator();
            while (propIt.current()) {
                if (propIt.current()->getType() == Property::PASSWORD) {
                    propIt.current()->updatePasswordStrength();
                    progress.setProgress(progr++);
                    qDebug() << CURRENT_FUNCTION << "progr =" << progr;
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
            *error = false;

    } catch (const PasswordCheckException& e) {
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
    try {
        if (m_showPasswordStrength) {
            Q3ListViewItemIterator it(this);
            while (it.current()) {
                Property::PasswordStrength strength =
                    boost::polymorphic_cast<TreeEntry*>(it.current())->weakestChildrenPassword();

                switch (strength) {
                    case Property::PWeak:
                        it.current()->setPixmap(0, QPixmap(":/images/traffic_red_16.png"));
                        break;

                    case Property::PAcceptable:
                        it.current()->setPixmap(0, QPixmap(":/images/traffic_yellow_16.png"));
                        break;

                    case Property::PStrong:
                        it.current()->setPixmap(0, QPixmap(":/images/traffic_green_16.png"));
                        break;

                    case Property::PUndefined:
                        it.current()->setPixmap(0, QPixmap(":/images/traffic_gray_16.png"));
                        break;

                    default:
                        qDebug() << CURRENT_FUNCTION << "Value out of range:" << strength;
                        break;
                }
                ++it;
            }
        } else {
            Q3ListViewItemIterator it(this);
            while (it.current()) {
                it.current()->setPixmap(0, QPixmap());
                ++it;
            }
        }
    } catch (const PasswordCheckException& e) {
        QMessageBox::warning(this, "QPaMaT", tr("<qt><nobr>Failed to calculate the password "
            "strength. The error message</nobr> was:<p>%1<p>Check your configuration!</qt>")
            .arg(e.what()), QMessageBox::Ok, QMessageBox::NoButton);
    }
}

// vim: set sw=4 ts=4 et:
