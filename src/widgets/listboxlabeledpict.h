/*
 * Id: $Id: listboxlabeledpict.h,v 1.1 2004/01/03 23:40:12 bwalle Exp $
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
#ifndef LISTBOXLABELEDPICT_H
#define LISTBOXLABELEDPICT_H

#include <qlistbox.h>
#include <qpainter.h>
#include <qpixmap.h>

class ListBoxLabeledPict : public QListBoxItem
{
    public:
        ListBoxLabeledPict(QListBox* listbox, const QPixmap& pixmap, 
            const QString& string);
        
        ListBoxLabeledPict(const QPixmap& pixmap, const QString& text);
        
        ListBoxLabeledPict(QListBox* listbox, const QPixmap& pixmap, 
            const QString& string, QListBoxItem *after );
        
    public:
        const QPixmap* pixmap() const;

        int	height(const QListBox* box) const;
        int	width(const QListBox* box) const;

        int rtti() const;

    protected:
        void paint(QPainter* painter);

    private:
        QPixmap m_pixmap;

    private:
        static const int RTTI;
};

#endif // LISTBOXLABELEDPICT_H
