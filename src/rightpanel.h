/*
 * Id: $Id: rightpanel.h,v 1.6 2003/12/16 22:52:47 bwalle Exp $
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
#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <qlistview.h>
#include <qframe.h>

#include "treeentry.h"
#include "southpanel.h"
#include "rightlistview.h"

class Qpamat;

/*!
 * \brief Represents the panel on the right which contains the entry list.
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.6 $
 * \date $Date: 2003/12/16 22:52:47 $
 */
class RightPanel : public QFrame
{
    Q_OBJECT
    
    public:
        /*!
         * Creates a new instance of the right panel.
         * \param parent the parent widget as usual for QObject
         */
        RightPanel(Qpamat* parent);
        
        /*!
         * Deletes the object
         */
        virtual ~RightPanel() { }
        
         /*!
         * Returns if the focus is inside this object.
         * \return \c true if the focus is inside this object, \c false otherwise.
         */
        bool isFocusInside() const;
        
    public slots:
        /*!
         * Sets the current item. The item must be a TreeEntry, this type is only for
         * the signal and slots mechanism.
         * \param item the current item
         */
        void setItem(QListViewItem* item);
        
        /*!
         * Clears the widget
         */
        void clear();
        
        /*!
         * Enables or diables the panel.
         * \param enabled \c true if the panel should be enabled, \c false otherwise
         */
        void setEnabled(bool enabled);
        
        /*!
         * Deletes the current item if the widget has the focus inside, i.e. if isFocusInside()
         * returns \c true.
         */
        void deleteCurrent();
        
        /*!
         * Inserts an item at the current position if the widget has the focus.
         */
        void insertAtCurrentPos();
        
    signals:
        
        /*!
         * If something was modified, need to determine if saving is necessary.
         */
        void stateModified();
        
        /*!
         * This signal is emitted if the password field got this focus.
         * \param focus \c true if it got the focus, \c false if the focus is lost
         */
        void passwordLineEditGotFocus(bool focus);
        
    private slots:
        void selectionChangeHandler(QListViewItem* item);
    
    private:
        TreeEntry* m_currentItem;
        RightListView* m_listView;
        SouthPanel* m_southPanel;
        Property* m_currentPropery;
    
};

#endif // RIGHTPANEL_H
