/*
 * Id: $Id: rightpanel.h,v 1.3 2003/11/29 14:43:03 bwalle Exp $
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

/*!
 * \brief Represents the panel on the right which contains the entry list.
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.3 $
 * \date $Date: 2003/11/29 14:43:03 $
 */
class RightPanel : public QFrame
{
    Q_OBJECT
    
    public:
        /*!
         * Creates a new instance of the right panel.
         * \param parent the parent widget as usual for QObject
         */
        RightPanel(QWidget* parent);
        
        /*!
         * Deletes the object
         */
        virtual ~RightPanel() { }
        
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
        
    private slots:
        void selectionChangeHandler(QListViewItem* item);
        void itemAppendedHandler();
    
    private:
        TreeEntry* m_currentItem;
        RightListView* m_listView;
        SouthPanel* m_southPanel;
        Property* m_currentPropery;
    
};

#endif // RIGHTPANEL_H
