/*
 * Id: $Id: rightlistview.cpp,v 1.12 2004/02/09 19:32:23 bwalle Exp $
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
#include <qlistview.h>
#include <qwidget.h>
#include <qheader.h>
#include <qclipboard.h>
#include <qapplication.h>
#include <qevent.h>
#include <qregexp.h>
#include <qstringlist.h>

#include "qpamat.h"
#include "global.h"
#include "rightlistview.h"
#include "dialogs/showpassworddialog.h"
#include "images/eye_16x16.xpm"
#include "images/edit_remove_16x16.xpm"
#include "images/edit_add_16x16.xpm"
#include "images/copy_16x16.xpm"
#include "help.h"


/*!
    \class RightListView
    
    \brief Represents the list view on the right where the key-value pairs are displayed.
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.12 $
    \date $Date: 2004/02/09 19:32:23 $
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
    : QListView(parent)
{
    addColumn(tr("Key"), 250);
    addColumn(tr("Value"), 250);
    
    setResizeMode(LastColumn);
    header()->setMovingEnabled(false);
    setSorting(-1);
    setSortColumn(-1);
    setAllColumnsShowFocus(true);
    
    initContextMenu();
    
    connect(this, SIGNAL(contextMenuRequested(QListViewItem*, const QPoint&, int)), 
        SLOT(showContextMenu(QListViewItem*, const QPoint&)));
    connect(this, SIGNAL(itemAppended()), SLOT(updateView()));
    connect(this, SIGNAL(itemAppended()), SLOT(itemAppendedHandler()));
    //connect(this, SIGNAL(itemDeleted(int)), SLOT(updateView()));
    connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
        SLOT(doubleClickHandler(QListViewItem*)));
    connect(this, SIGNAL(itemDeleted(int)), SIGNAL(stateModified()));
    connect(this, SIGNAL(itemAppended()), SIGNAL(stateModified()));
    connect(this, SIGNAL(selectionChanged()), SLOT(setMoveStateCorrect()));
    connect(this, SIGNAL(currentChanged(QListViewItem*)), SLOT(setMoveStateCorrect()));
    connect(this, SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint&, int)),
        SLOT(mouseButtonClickedHandler(int, QListViewItem*, const QPoint&, int)));
}


/*!
    Initializes the context menu.
*/
void RightListView::initContextMenu()
{
    m_contextMenu = new QPopupMenu(this);
    m_contextMenu->insertItem(QIconSet(edit_add_16x16_xpm), tr("&New"), NEW);
    m_contextMenu->insertItem(QIconSet(edit_remove_16x16_xpm), 
        tr("&Delete") + "\t" + QString(QKeySequence(Key_Delete)), DELETE);
    m_contextMenu->insertSeparator();
    m_contextMenu->insertItem(QIconSet(copy_16x16_xpm),
        tr("&Copy") + "\t" + QString(QKeySequence(CTRL|Key_C)), COPY);
    m_contextMenu->insertItem(QIconSet(eye_16x16_xpm), tr("Show &password..."), SHOW_PW);
}


/*!
    Shows the context Menu. Usually connected to QListView::contextMenuRequested.
    \param item the list view item
    \param point the coordinates of the click
*/
void RightListView::showContextMenu(QListViewItem* item, const QPoint& point) 
{
    bool passw = item != 0 && m_currentItem->getProperty(item->text(2).toInt(0))->getType()
        == Property::PASSWORD;
    
    m_contextMenu->setItemEnabled(DELETE, item != 0);
    m_contextMenu->setItemEnabled(COPY, item != 0);
    m_contextMenu->setItemEnabled(SHOW_PW, passw);
    
    int id = m_contextMenu->exec(point);
    switch (id)
    {
        case DELETE:
        {
            int num = item->text(2).toInt(0);
            m_currentItem->deleteProperty(num);
            emit itemDeleted(num);
            break;
        }
        case COPY:
            copyItem(item);
            break;
        case NEW:
            m_currentItem->appendProperty(new Property());
            emit stateModified();
            break;
        case SHOW_PW:
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
    
    QListViewItem* item = firstChild();
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
    QListViewItem* selected = selectedItem();
    int key = evt->key();
    
    if (key == Key_Delete)
    {
        deleteCurrent();
    }
    else if (selected && key == Key_C && evt->state() == ControlButton)
    {
        copyItem(selected);
    }
    else
    {
        evt->ignore();
        QListView::keyPressEvent(evt);
    }
}


/*!
    Copies a item into the clipboard.
    \param item
*/
void RightListView::copyItem(QListViewItem* item)
{
    if (item != 0)
    {
        Property* property = m_currentItem->getProperty(item->text(2).toInt(0));
        QClipboard* clip = QApplication::clipboard();
        if (clip->supportsSelection())
        {
            clip->setText(property->getValue(), QClipboard::Selection);
        }
        clip->setText(property->getValue(), QClipboard::Clipboard);
    }
}


/*!
    Handles double clicks (show the password or open the browser).
    \param item the item
*/
void RightListView::doubleClickHandler(QListViewItem* item)
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
void RightListView::mouseButtonClickedHandler(int but, QListViewItem* item, 
                                              const QPoint& point, int col)
{
    if (but == MidButton)
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
            new QListViewItem(this, lastItem(), current->getKey(), current->getVisibleValue(), 
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
    QListViewItem* item = selectedItem();
    
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
void RightListView::setItem(QListViewItem* item)
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
    QListViewItem* selected = selectedItem();
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
    QListViewItem* addedItem = lastItem();
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
    QListViewItem* selected = selectedItem();
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
    QListViewItem* selected = selectedItem();
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
    QListViewItem* selected = selectedItem();
    
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
