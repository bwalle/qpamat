/*
 * Id: $Id: rightlistview.h,v 1.1 2003/10/20 20:55:13 bwalle Exp $
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
#ifndef RIGHTLISTVIEW_H
#define RIGHTLISTVIEW_H

#include <qlistview.h>
#include <qwidget.h>
#include <qevent.h>
#include <qpopupmenu.h>

#include "property.h"
#include "treeentry.h"

/**
 * Represents the list view on the right where the key-value pairs are displayed.
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/10/20 20:55:13 $
 */
class RightListView : public QListView
{
    Q_OBJECT
    
    public:
        
        /**
         * Enumeration for menu.
         */
        enum MenuID { NEW, DELETE, COPY };
    
        /**
         * Creates a new object of the list view.
         * @param parent the parent widget
         */
        RightListView(QWidget* parent);
        
        /**
         * Deletes the object.
         */
        virtual ~RightListView() { }
        
        /**
         * Key press.
         */
        void keyPressEvent(QKeyEvent* evt);
        
        /**
         * Sets the current item. The item must be a TreeEntry, this type is only for
         * the signal and slots mechanism.
         * @param the current item
         */
        void setItem(QListViewItem* item);
    
    private slots:
        void updateView();
        void updateSelected(Property* property);
        void showContextMenu(QListViewItem* item, const QPoint& point);
        void copyItem(QListViewItem* item);
        void doubleClickHandler(QListViewItem* item);
    
    signals:
        void itemAppended();
        void itemDeleted();
        
    private:
        void initContextMenu();
        
    private:
        TreeEntry* m_currentItem;
        QPopupMenu* m_contextMenu;
   
}; 

#endif // RIGHTLISTVIEW_H
