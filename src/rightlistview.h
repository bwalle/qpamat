/*
 * Id: $Id: rightlistview.h,v 1.7 2003/12/29 15:12:27 bwalle Exp $
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

class RightListView : public QListView
{
    Q_OBJECT
    
    public:
        enum MenuID 
        { 
            NEW, DELETE, COPY, SHOW_PW 
        };
        
    public:
        RightListView(QWidget* parent);
        
        void setItem(QListViewItem* item);
        bool isFocusInside() const;
        void setSelectedIndex(uint index);
        
    public slots:
        void deleteCurrent();
        void insertAtCurrentPos();
        
        void moveDown();
        void moveUp();
        
    protected:
        void keyPressEvent(QKeyEvent* evt);
        
    private slots:
        void updateView();
        void updateSelected(Property* property);
        void showContextMenu(QListViewItem* item, const QPoint& point);
        void copyItem(QListViewItem* item);
        void doubleClickHandler(QListViewItem* item);
        void itemAppendedHandler();
        void setMoveStateCorrect();
        void mouseButtonClickedHandler(int but, QListViewItem* item, const QPoint& point, int c);
        
    signals:
        void itemAppended();
        void itemDeleted();
        void stateModified();
        void enableMoving(bool up, bool down);
        
    private:
        void initContextMenu();
        
    private:
        TreeEntry*  m_currentItem;
        QPopupMenu* m_contextMenu;
   
}; 

QTextStream& operator<<(QTextStream& ts, const RightListView& listview);
QTextStream& operator>>(QTextStream& ts, RightListView& listview);

#endif // RIGHTLISTVIEW_H
