/*
 * Id: $Id: rightlistview.cpp,v 1.1 2003/10/20 20:55:13 bwalle Exp $
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

#include "rightlistview.h"
#include "../images/delete_16x16.xpm"
#include "../images/new_16x16.xpm"
#include "../images/copy_16x16.xpm"
#include "help.h"

// -------------------------------------------------------------------------------------------------
RightListView::RightListView(QWidget* parent)
// -------------------------------------------------------------------------------------------------
    : QListView(parent)
{
    addColumn(tr("Key"));
    addColumn(tr("Value"));
    
    header()->setStretchEnabled(true);
    header()->setMovingEnabled(false);
    setSorting(-1);
    setSortColumn(-1);
    setAllColumnsShowFocus(true);
    
    initContextMenu();
    
    connect(this, SIGNAL(contextMenuRequested(QListViewItem*, const QPoint&, int)), 
        this, SLOT(showContextMenu(QListViewItem*, const QPoint&)));
    connect(this, SIGNAL(itemAppended()), this, SLOT(updateView()));
    connect(this, SIGNAL(itemDeleted()), this, SLOT(updateView()));
    connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
        this, SLOT(doubleClickHandler(QListViewItem*)));
}


// -------------------------------------------------------------------------------------------------
void RightListView::initContextMenu()
// -------------------------------------------------------------------------------------------------
{
    m_contextMenu = new QPopupMenu(this);
    m_contextMenu->insertItem(QIconSet(new_16x16_xpm), tr("&New"), NEW);
    m_contextMenu->insertItem(QIconSet(delete_16x16_xpm), 
        tr("&Delete") + "\t" + QString(QKeySequence(Key_Delete)), DELETE);
    m_contextMenu->insertSeparator();
    m_contextMenu->insertItem(QIconSet(copy_16x16_xpm),
        tr("&Copy") + "\t" + QString(QKeySequence(CTRL|Key_C)), COPY);
}


// -------------------------------------------------------------------------------------------------
void RightListView::showContextMenu(QListViewItem* item, const QPoint& point) 
// -------------------------------------------------------------------------------------------------
{
    int id = m_contextMenu->exec(point);
    
    switch (id)
    {
        case DELETE:
            m_currentItem->deleteProperty(item->text(2).toInt(0));
            emit itemDeleted();
            break;
        case COPY:
            copyItem(item);
            break;
        case NEW:
            m_currentItem->appendProperty(new Property());
            break;
    };
}


// -------------------------------------------------------------------------------------------------
void RightListView::keyPressEvent(QKeyEvent* evt)
// -------------------------------------------------------------------------------------------------
{
    QListViewItem* current = currentItem();
    int key = evt->key();
    
    if (current != 0)
    {
        if (key == Key_Delete)
        {
            m_currentItem->deleteProperty(current->text(2).toInt(0));
            emit itemDeleted();
        }
        else if (key == Key_C && evt->state() == ControlButton)
        {
            copyItem(current);
        }
        else
        {
            evt->ignore();
            QListView::keyPressEvent(evt);
        }
    }
}


// -------------------------------------------------------------------------------------------------
void RightListView::copyItem(QListViewItem* item)
// -------------------------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------------------------
void RightListView::doubleClickHandler(QListViewItem* item)
// -------------------------------------------------------------------------------------------------
{
    if (item != 0)
    {
        Property* property = m_currentItem->getProperty(item->text(2).toInt(0));
        if (property->getType() == Property::URL)
        {
            Help::openURL(this, property->getValue());
        }
        else
        {
            copyItem(item);
        }
    }
}

// -------------------------------------------------------------------------------------------------
void RightListView::updateView()
// -------------------------------------------------------------------------------------------------
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

// -------------------------------------------------------------------------------------------------
void RightListView::updateSelected(Property* property)
// -------------------------------------------------------------------------------------------------
{
    QListViewItem* item = selectedItem();
    
    if (item != 0)
    {
        item->setText(0, property->getKey());
        QString value = property->getValue();
        item->setText(1, property->getVisibleValue());
    }
}


// -------------------------------------------------------------------------------------------------
void RightListView::setItem(QListViewItem* item)
// -------------------------------------------------------------------------------------------------
{
    disconnect(this, SIGNAL(itemAppended()));
    m_currentItem = dynamic_cast<TreeEntry*>(item);
    
    if (m_currentItem == 0)
    {
        return;
    }
    
    connect(m_currentItem, SIGNAL(propertyAppended()), this, SIGNAL(itemAppended()));
    updateView();
}

