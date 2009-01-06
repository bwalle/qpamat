/*
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

#include <Q3ListView>
#include <Q3Frame>
#include <QTextStream>

#include "treeentry.h"
#include "southpanel.h"
#include "rightlistview.h"

class Qpamat;

class RightPanel : public Q3Frame
{
    Q_OBJECT

    public:
        friend QTextStream& operator<<(QTextStream& ts, const RightPanel& panel);
        friend QTextStream& operator>>(QTextStream& ts, RightPanel& panel);

    public:
        RightPanel(Qpamat* parent);
        bool isFocusInside() const;

    public slots:
        void setItem(Q3ListViewItem* item);
        void clear();
        void setEnabled(bool enabled);
        void deleteCurrent();
        void insertAtCurrentPos();

    protected:
        void clear(bool full);

    signals:
        void stateModified();
        void passwordLineEditGotFocus(bool focus);
        void passwordStrengthUpdated();

    private slots:
        void selectionChangeHandler(Q3ListViewItem* item);
        void itemDeletedHandler(int item);

    private:
        QLabel          *m_overviewLabel;
        TreeEntry       *m_currentItem;
        RightListView   *m_listView;
        SouthPanel      *m_southPanel;
        Property        *m_currentPropery;
};

QTextStream& operator<<(QTextStream& ts, const RightPanel& panel);
QTextStream& operator>>(QTextStream& ts, RightPanel& panel);

#endif // RIGHTPANEL_H

// :maxLineLen=100:
