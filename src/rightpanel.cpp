/*
 * Id: $Id: rightpanel.cpp,v 1.1 2003/10/20 20:55:13 bwalle Exp $
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
#include <qlayout.h>
#include <qheader.h>
#include <qlabel.h>
#include <qframe.h>

#include "rightpanel.h"
#include "rightlistview.h"
#include "property.h"
#include "treeentry.h"
#include "southpanel.h"

// -------------------------------------------------------------------------------------------------
RightPanel::RightPanel(QWidget* parent) : QFrame(parent, "RightPanel")
// -------------------------------------------------------------------------------------------------
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_listView = new RightListView(this);
    m_southPanel = new SouthPanel(this);
    layout->addWidget(m_listView);
    layout->setStretchFactor(m_listView, 10);
    layout->addWidget(m_southPanel);
    
    setEnabled(false);
    
    connect(m_listView, SIGNAL(selectionChanged(QListViewItem*)),
        this, SLOT(selectionChangeHandler(QListViewItem*)));
    connect(m_listView, SIGNAL(itemAppended()), this, SLOT(itemAppendedHandler()));
    connect(m_listView, SIGNAL(itemDeleted()), m_southPanel, SLOT(clear()));
}


// -------------------------------------------------------------------------------------------------
void RightPanel::clear()
// -------------------------------------------------------------------------------------------------
{
    m_listView->clear();
    m_southPanel->clear();
    setEnabled(false);
}


// -------------------------------------------------------------------------------------------------
void RightPanel::itemAppendedHandler()
// -------------------------------------------------------------------------------------------------
{
    QListViewItem* addedItem = m_listView->lastItem();
    m_listView->setSelected(addedItem, true);
    m_southPanel->setFocus();
}


// -------------------------------------------------------------------------------------------------
void RightPanel::setEnabled(bool enabled)
// -------------------------------------------------------------------------------------------------
{
    QFrame::setEnabled(enabled);
    m_listView->setEnabled(enabled);
}


// -------------------------------------------------------------------------------------------------
void RightPanel::setItem(QListViewItem* item)
// -------------------------------------------------------------------------------------------------
{
    clear();
    m_currentItem = dynamic_cast<TreeEntry*>(item);
    
    if (m_currentItem == 0)
    {
        return;
    }
    
    m_listView->setItem(m_currentItem);
    
    setEnabled(!m_currentItem->isCategory());
}


// -------------------------------------------------------------------------------------------------
void RightPanel::selectionChangeHandler(QListViewItem* item)
// -------------------------------------------------------------------------------------------------
{
    disconnect(this, SLOT(updateSelected(Property*)));
    
    Property* currentProperty = m_currentItem->getProperty(item->text(2).toInt(0));
    m_southPanel->setItem(currentProperty);
    
    connect(currentProperty, SIGNAL(propertyChanged(Property*)),
        m_listView, SLOT(updateSelected(Property*)));
}
