/*
 * Id: $Id$
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
#include <Q3ListView>
#include <QLayout>
#include <Q3Header>
#include <QLabel>
#include <Q3Frame>
#include <QVBoxLayout>
#include <QTextStream>

#include "qpamat.h"

#include "rightpanel.h"
#include "rightlistview.h"
#include "property.h"
#include "treeentry.h"
#include "southpanel.h"


/*!
    \class RightPanel
    
    \brief Represents the panel on the right which contains the entry list.
    
    \ingroup gui
    
    \author Bernhard Walle
    \version $Revision: 1.6 $
    \date $Date$
*/

/*!
    \fn RightPanel::stateModified()
    
    If something was modified, need to determine if saving is necessary.
*/

/*!
    \fn RightPanel::passwordLineEditGotFocus(bool)
    
    This signal is emitted if the password field got this focus.
    \param focus \c true if it got the focus, \c false if the focus is lost
*/

/*!
    \fn RightPanel::passwordStrengthUpdated()
    
    This signal is emitted if the password strength of an item has changed an therefore
    the displaying must be updated.
*/

/*!
    Creates a new instance of the right panel.
    \param parent the parent widget as usual for QObject
*/
RightPanel::RightPanel(Qpamat* parent) : Q3Frame(parent, "RightPanel")
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_listView = new RightListView(this);
    m_southPanel = new SouthPanel(this);
    layout->addWidget(m_listView);
    layout->setStretchFactor(m_listView, 10);
    layout->addWidget(m_southPanel);
    
    setEnabled(false);
    
    connect(m_listView, SIGNAL(selectionChanged(Q3ListViewItem*)),
        this, SLOT(selectionChangeHandler(Q3ListViewItem*)));
    connect(m_listView, SIGNAL(itemDeleted(int)), SLOT(itemDeletedHandler(int)));
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
    connect(m_southPanel, SIGNAL(passwordStrengthUpdated()), SIGNAL(passwordStrengthUpdated()));
}


/*!
    Clears the widget
*/
void RightPanel::clear()
{
    m_listView->clear();
    m_southPanel->clear();
    setEnabled(false);
}


/*!
    Handler for deleted widgets. Selects the next item.

    \param item the item number that was deleted.

*/
void RightPanel::itemDeletedHandler(int item)
{
    m_listView->updateView();
    int numberOfChilds = m_listView->childCount();
    if (numberOfChilds == 0)
    {
        m_southPanel->clear();
        return;
    }

    PRINT_TRACE("item = %d, numberOfChilds = %d", item, numberOfChilds);
    
    // if the last one was deleted, select the previous one
    if (item == numberOfChilds)
    {
        --item;
    }

    Q3ListViewItem* it = m_listView->firstChild();
    for (int i = 0; i < item; ++i)
    {
        it = it->nextSibling();
    }

    m_listView->setSelected(it, true);
}


/*!
    Enables or diables the panel.
    \param enabled \c true if the panel should be enabled, \c false otherwise
*/
void RightPanel::setEnabled(bool enabled)
{
    Q3Frame::setEnabled(enabled);
    m_listView->setEnabled(enabled);
}


/*!
    Sets the current item. The item must be a TreeEntry, this type is only for
    the signal and slots mechanism.
    \param item the current item
*/
void RightPanel::setItem(Q3ListViewItem* item)
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


/*!
    Handles changing of selections.
    \param item the item
*/
void RightPanel::selectionChangeHandler(Q3ListViewItem* item)
{
    disconnect(m_listView, SLOT(updateSelected(Property*)));
    
    Property* currentProperty = m_currentItem->getProperty(item->text(2).toInt(0));
    m_southPanel->setItem(currentProperty);
    
    connect(currentProperty, SIGNAL(propertyChanged(Property*)), 
        m_listView, SLOT(updateSelected(Property*)));
}


/*!
    Deletes the current item if the widget has the focus inside, i.e. if isFocusInside()
    returns \c true.
*/
void RightPanel::deleteCurrent()
{
    if (isFocusInside())
    {
        m_listView->deleteCurrent();
    }
}


/*!
    Inserts an item at the current position if the widget has the focus.
*/
void RightPanel::insertAtCurrentPos()
{
    if (isFocusInside())
    {
        m_listView->insertAtCurrentPos();
    }
}


/*!
    Returns if the focus is inside this object.
    \return \c true if the focus is inside this object, \c false otherwise.
*/
bool RightPanel::isFocusInside() const
{
    return m_listView->isFocusInside() || m_southPanel->isFocusInside();
}


/*!
    \relates RightPanel
    
    Stores the width of the columns of the listview.
*/
QTextStream& operator<<(QTextStream& ts, const RightPanel& panel)
{
    ts << *panel.m_listView;
    return ts;
}


/*!
    \relates RightPanel
    
    Reads the width of the columns of the listview.
*/
QTextStream& operator>>(QTextStream& ts, RightPanel& panel)
{
    ts >> *panel.m_listView;
    return ts;
}

// :maxLineLen=100:
