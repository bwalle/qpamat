/*
 * Id: $Id: rightpanel.cpp,v 1.4 2003/12/16 22:52:47 bwalle Exp $
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

#include "qpamat.h"
#include "rightpanel.h"
#include "rightlistview.h"
#include "property.h"
#include "treeentry.h"
#include "southpanel.h"

// -------------------------------------------------------------------------------------------------
RightPanel::RightPanel(Qpamat* parent) : QFrame(parent, "RightPanel")
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
    connect(m_listView, SIGNAL(itemDeleted()), m_southPanel, SLOT(clear()));
    connect(m_listView, SIGNAL(stateModified()), SIGNAL(stateModified()));
    connect(m_southPanel, SIGNAL(moveUp()), m_listView, SLOT(moveUp()));
    connect(m_southPanel, SIGNAL(moveDown()), m_listView, SLOT(moveDown()));
    connect(m_listView, SIGNAL(enableMoving(bool, bool)), 
        m_southPanel, SLOT(setMovingEnabled(bool, bool)));
    connect(m_southPanel, SIGNAL(passwordLineEditGotFocus(bool)), 
        SIGNAL(passwordLineEditGotFocus(bool)));
    connect(parent, SIGNAL(insertPassword(const QString&)), 
        m_southPanel, SLOT(insertPassword(const QString&)));
    connect(m_southPanel, SIGNAL(stateModified()), SIGNAL(stateModified()));
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
    disconnect(m_listView, SLOT(updateSelected(Property*)));
    
    Property* currentProperty = m_currentItem->getProperty(item->text(2).toInt(0));
    m_southPanel->setItem(currentProperty);
    
    connect(currentProperty, SIGNAL(propertyChanged(Property*)), 
        m_listView, SLOT(updateSelected(Property*)));
}


// -------------------------------------------------------------------------------------------------
void RightPanel::deleteCurrent()
// -------------------------------------------------------------------------------------------------
{
    if (isFocusInside())
    {
        m_listView->deleteCurrent();
    }
}


// -------------------------------------------------------------------------------------------------
void RightPanel::insertAtCurrentPos()
// -------------------------------------------------------------------------------------------------
{
    if (isFocusInside())
    {
        m_listView->insertAtCurrentPos();
    }
}


// -------------------------------------------------------------------------------------------------
bool RightPanel::isFocusInside() const
// -------------------------------------------------------------------------------------------------
{
    return m_listView->isFocusInside() || m_southPanel->isFocusInside();
}
