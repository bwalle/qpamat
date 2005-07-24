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
#ifndef RIGHTLISTVIEW_H
#define RIGHTLISTVIEW_H

#include <Q3ListView>
#include <QWidget>
#include <QEvent>
#include <Q3PopupMenu>
#include <QTextStream>
#include <QKeyEvent>

#include "property.h"
#include "treeentry.h"

class RightListView : public Q3ListView
{
    Q_OBJECT

    public:
        enum MenuID 
        { 
            NEW, DELETE, COPY, SHOW_PW 
        };
        
    public:
        RightListView(QWidget* parent);
        
        void setItem(Q3ListViewItem* item);
        bool isFocusInside() const;
        void setSelectedIndex(uint index);
        
    public slots:
        void deleteCurrent();
        void insertAtCurrentPos();

        void updateView();
        
        void moveDown();
        void moveUp();
        
    protected:
        void keyPressEvent(QKeyEvent* evt);
        
    private slots:
        void updateSelected(Property* property);
        void showContextMenu(Q3ListViewItem* item, const QPoint& point);
        void copyItem(Q3ListViewItem* item);
        void doubleClickHandler(Q3ListViewItem* item);
        void itemAppendedHandler();
        void setMoveStateCorrect();
        void mouseButtonClickedHandler(int but, Q3ListViewItem* item, const QPoint& point, int c);
        
    signals:
        void itemAppended();
        void itemDeleted(int item);
        void stateModified();
        void enableMoving(bool up, bool down);
        
    private:
        void initContextMenu();
        
    private:
        TreeEntry*  m_currentItem;
        Q3PopupMenu* m_contextMenu;
   
}; 

QTextStream& operator<<(QTextStream& ts, const RightListView& listview);
QTextStream& operator>>(QTextStream& ts, RightListView& listview);

#endif // RIGHTLISTVIEW_H
