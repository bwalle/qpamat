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
#include <Q3ListView>
#include <QWidget>
#include <Q3Header>
#include <QClipboard>
#include <QApplication>
#include <QEvent>
#include <QRegExp>
#include <QStringList>
#include <QPixmap>
#include <QKeyEvent>
#include <QTextStream>
#include <Q3PopupMenu>

#include "qpamat.h"

#include "global.h"
#include "rightlistview.h"
#include "dialogs/showpassworddialog.h"
#include "help.h"


/*!
    \class RightListView
    
    \brief Represents the list view on the right where the key-value pairs are displayed.
    \ingroup gui
    \author Bernhard Walle
*/

/*!
    \enum RightListView::MenuID
    
    Enumeration for menu.
*/

/*!
    \fn RightListView::itemAppended()
    
    This signal is emitted if an item was appended.
*/

/*!
    \fn RightListView::itemDeleted(int)
    
    This signal is emitted if an item was deleted.

    \param item the number of the deleted item (first item = 0)
*/

/*!
    \fn RightListView::stateModified()
    
    If something was modified, need to determine if saving is necessary.
*/

/*!
    \fn RightListView::enableMoving(bool, bool)
    
    This signal is emitted always if the selection changes.
    \param up if moving up is enabled
    \param down if moving down is enabled
*/

/*!
    Creates a new object of the list view.
    \param parent the parent widget
*/
RightListView::RightListView(QWidget* parent)
    : Q3ListView(parent)
{
    addColumn(tr("Key"), 250);
    addColumn(tr("Value"), 250);
    
    setResizeMode(LastColumn);
    header()->setMovingEnabled(false);
    setSorting(-1);
    setSortColumn(-1);
    setAllColumnsShowFocus(true);
    
    initContextMenu();
    
    connect(this, SIGNAL(contextMenuRequested(Q3ListViewItem*, const QPoint&, int)), 
        SLOT(showContextMenu(Q3ListViewItem*, const QPoint&)));
    connect(this, SIGNAL(itemAppended()), SLOT(updateView()));
    connect(this, SIGNAL(itemAppended()), SLOT(itemAppendedHandler()));
    //connect(this, SIGNAL(itemDeleted(int)), SLOT(updateView()));
    connect(this, SIGNAL(doubleClicked(Q3ListViewItem*, const QPoint&, int)),
        SLOT(doubleClickHandler(Q3ListViewItem*)));
    connect(this, SIGNAL(itemDeleted(int)), SIGNAL(stateModified()));
    connect(this, SIGNAL(itemAppended()), SIGNAL(stateModified()));
    connect(this, SIGNAL(selectionChanged()), SLOT(setMoveStateCorrect()));
    connect(this, SIGNAL(currentChanged(Q3ListViewItem*)), SLOT(setMoveStateCorrect()));
    connect(this, SIGNAL(mouseButtonClicked(int, Q3ListViewItem*, const QPoint&, int)),
        SLOT(mouseButtonClickedHandler(int, Q3ListViewItem*, const QPoint&, int)));
}


/*!
    Initializes the context menu.
*/
void RightListView::initContextMenu()
{
    m_contextMenu = new Q3PopupMenu(this);
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/stock_add_16.png")), tr("&New"), M_NEW);
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/stock_remove_16.png")), 
        tr("&Delete") + "\t" + QString(QKeySequence(Qt::Key_Delete)), M_DELETE);
    m_contextMenu->insertSeparator();
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/stock_copy_16.png")),
        tr("&Copy") + "\t" + QString(QKeySequence(Qt::CTRL|Qt::Key_C)), M_COPY);
    m_contextMenu->insertItem(QIcon(QPixmap(":/images/eye_16.png")), 
        tr("Show &password..."), M_SHOW_PW);
}


/*!
    Shows the context Menu. Usually connected to QListView::contextMenuRequested.
    \param item the list view item
    \param point the coordinates of the click
*/
void RightListView::showContextMenu(Q3ListViewItem* item, const QPoint& point) 
{
    if (!isEnabled())
    {
        return;
    }
    bool passw = item != 0 && m_currentItem->getProperty(item->text(2).toInt(0))->getType()
        == Property::PASSWORD;
    
    m_contextMenu->setItemEnabled(M_DELETE, item != 0);
    m_contextMenu->setItemEnabled(M_COPY, item != 0);
    m_contextMenu->setItemEnabled(M_SHOW_PW, passw);
    
    int id = m_contextMenu->exec(point);
    switch (id)
    {
        case M_DELETE:
        {
            int num = item->text(2).toInt(0);
            m_currentItem->deleteProperty(num);
            emit itemDeleted(num);
            break;
        }
        case M_COPY:
            copyItem(item);
            break;
        case M_NEW:
            m_currentItem->appendProperty(new Property());
            emit stateModified();
            break;
        case M_SHOW_PW:
            ShowPasswordDialog* dlg = new ShowPasswordDialog(this, ShowPasswordDialog::TNormalPasswordDlg);
            dlg->setPassword(m_currentItem->getProperty(item->text(2).toInt(0))->getValue());
            dlg->exec();
            delete dlg;
    };
}


/*!
    Sets the selected index.
    \param index the index
*/
void RightListView::setSelectedIndex(uint index)
{
    Q_ASSERT(index <= uint(childCount()));
    
    Q3ListViewItem* item = firstChild();
    for (uint i = 0; i < index; ++i)
    {
        item = item->nextSibling();
    }
    
    setSelected(item, true);
}


/*!
    Called if the user presses a key
    \param evt the event
*/
void RightListView::keyPressEvent(QKeyEvent* evt)
{
    Q3ListViewItem* selected = selectedItem();
    int key = evt->key();
    
    if (key == Qt::Key_Delete)
    {
        deleteCurrent();
    }
    else if (selected && key == Qt::Key_C && evt->state() == Qt::ControlButton)
    {
        copyItem(selected);
    }
    else
    {
        evt->ignore();
        Q3ListView::keyPressEvent(evt);
    }
}


/*!
    Copies a item into the clipboard.
    \param item
*/
void RightListView::copyItem(Q3ListViewItem* item)
{
    if (item != 0)
    {
        Property* property = m_currentItem->getProperty(item->text(2).toInt(0));
        QClipboard* clip = QApplication::clipboard();
        clip->setText(property->getValue(), QClipboard::Clipboard);
        if (clip->supportsSelection())
        {
            clip->setText(property->getValue(), QClipboard::Selection);
        }
    }
}


/*!
    Handles double clicks (show the password or open the browser).
    \param item the item
*/
void RightListView::doubleClickHandler(Q3ListViewItem* item)
{
    if (item != 0)
    {
        Property* property = m_currentItem->getProperty(item->text(2).toInt(0));
        if (property->getType() == Property::URL)
        {
            Help::openURL(this, property->getValue());
        }
        else if (property->getType() == Property::PASSWORD)
        {
            ShowPasswordDialog* dlg = new ShowPasswordDialog(this, ShowPasswordDialog::TNormalPasswordDlg);
            dlg->setPassword(m_currentItem->getProperty(item->text(2).toInt(0))->getValue());
            dlg->exec();
            delete dlg;
        }
        else
        {
            qpamat->message(tr("Double click only supported with passwords and URLs!"));
        }
    }
}


/*!
    Handles middle clicks (copy into clipboard)
    \param but the mouse button
    \param item the listview item
    \param point the coordinares
    \param col the column
*/
void RightListView::mouseButtonClickedHandler(int but, Q3ListViewItem* item, 
                                              const QPoint& point, int col)
{
    UNUSED(point);
    UNUSED(col);

    if (but == Qt::MidButton)
    {
        copyItem(item);
    }
}

/*!
    Updates the view.
*/
void RightListView::updateView()
{
    clear();
    
    if (! m_currentItem->isCategory())
    {
        TreeEntry::PropertyIterator it = m_currentItem->propertyIterator();
        
        Property* current;
        int i = 0;
        while ( (current = it.current()) != 0 )
        {
            new Q3ListViewItem(this, lastItem(), current->getKey(), current->getVisibleValue(), 
                QString::number(i++));
            ++it;
        }
    }
    
    setEnabled(true);
}


/*!
    Updates the selected.
    \param property the property
*/
void RightListView::updateSelected(Property* property)
{
    Q3ListViewItem* item = selectedItem();
    
    if (item != 0)
    {
        item->setText(0, property->getKey());
        QString value = property->getValue();
        item->setText(1, property->getVisibleValue());
    }
}


/*!
    Sets an item
    \param item the item
*/
void RightListView::setItem(Q3ListViewItem* item)
{
    disconnect(this, SIGNAL(itemAppended()));
    m_currentItem = dynamic_cast<TreeEntry*>(item);
    if (m_currentItem != 0)
    {
        connect(m_currentItem, SIGNAL(propertyAppended()), this, SIGNAL(itemAppended()));
        updateView();
    }
}


/*!
    Deletes the current item.
*/
void RightListView::deleteCurrent()
{
    Q3ListViewItem* selected = selectedItem();
    if (selected)
    {
        int num = selected->text(2).toInt(0);
        m_currentItem->deleteProperty(num);
        emit itemDeleted(num);
    }
    else
    {
        qpamat->message(tr("No item selected!"));
    }
}


/*!
    Inserts a new item at the current position.
*/
void RightListView::insertAtCurrentPos()
{
    m_currentItem->appendProperty(new Property());
}


/*!
    Handler for appended items.
*/
void RightListView::itemAppendedHandler()
{
    Q3ListViewItem* addedItem = lastItem();
    if (addedItem)
    {
        setSelected(addedItem, true);
    }
}


/*!
    Asks if the focus is in a widget inside the widget.
    \return \c true if it is, \c false if not
*/
bool RightListView::isFocusInside() const
{
    return hasFocus();
}


/*!
    Moves the current property one down.
*/
void RightListView::moveDown()
{
    Q3ListViewItem* selected = selectedItem();
    if (selected)
    {
        int index = selected->text(2).toInt(0);
        // up in list terminology means greater index
        m_currentItem->movePropertyOneUp(index);
        updateView();
        setSelectedIndex(index+1);
        emit stateModified();
    }
    else
    {
        qpamat->message("No item selected");
    }
}


/*!
    Moves the current property one up.
*/
void RightListView::moveUp()
{
    Q3ListViewItem* selected = selectedItem();
    if (selected)
    {
        int index = selected->text(2).toInt(0);
        // up in list terminology means greater index
        m_currentItem->movePropertyOneDown(index);
        updateView();
        setSelectedIndex(index-1);
        emit stateModified();
    }
    else
    {
        qpamat->message("No item selected");
    }
}


/*!
    Sets the move state correct, i.e. whether moving up or down is enabled. Emits the 
    enableMoving() signal.
*/
void RightListView::setMoveStateCorrect()
{
    bool up = false;
    bool down = false;
    Q3ListViewItem* selected = selectedItem();
    
    if (selected)
    {
        int index = selected->text(2).toInt(0);
        int number = childCount();
        down = (index < number - 1);
        up = (index > 0);
    }
    emit enableMoving(up, down);
}


/*!
    \relates RightListView
    
    Writes the widths of the column to the text stream.
*/
QTextStream& operator<<(QTextStream& ts, const RightListView& listview)
{
    ts << "column0width=" << listview.columnWidth(0)
       << ",column1width=" << listview.columnWidth(1);
    return ts;
}


/*!
    \related RightListView
    
    Restores the widths of the columns from the text stream.
*/
QTextStream& operator>>(QTextStream& ts, RightListView& listview)
{
    QRegExp rx("column0width=(\\d+),column1width=(\\d+)");
    QString text;
    ts >> text;
    rx.search(text);
    QStringList list = rx.capturedTexts();
    if (list.size() != 3)
    {
        PRINT_DBG("Wrong input\n%s", text.latin1());
    }
    else
    {
        listview.setColumnWidth(0, list[1].toInt());
        listview.setColumnWidth(1, list[2].toInt()); 
    }
    return ts;
}

// :maxLineLen=100:
