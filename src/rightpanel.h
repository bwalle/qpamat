/*
 * Id: $Id: rightpanel.h,v 1.7 2003/12/29 15:12:27 bwalle Exp $
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
#include <qtextstream.h>

#include "treeentry.h"
#include "southpanel.h"
#include "rightlistview.h"

class Qpamat;

class RightPanel : public QFrame
{
    Q_OBJECT
    
    public:
        friend QTextStream& operator<<(QTextStream& ts, const RightPanel& panel);
        friend QTextStream& operator>>(QTextStream& ts, RightPanel& panel);
    
    public:
        RightPanel(Qpamat* parent);
        bool isFocusInside() const;
        
    public slots:
        void setItem(QListViewItem* item);
        void clear();
        void setEnabled(bool enabled);
        void deleteCurrent();
        void insertAtCurrentPos();
        
    signals:
        void stateModified();
        void passwordLineEditGotFocus(bool focus);
        void passwordStrengthUpdated();
        
    private slots:
        void selectionChangeHandler(QListViewItem* item);
    
    private:
        TreeEntry*      m_currentItem;
        RightListView*  m_listView;
        SouthPanel*     m_southPanel;
        Property*       m_currentPropery;
};

QTextStream& operator<<(QTextStream& ts, const RightPanel& panel);
QTextStream& operator>>(QTextStream& ts, RightPanel& panel);

#endif // RIGHTPANEL_H
