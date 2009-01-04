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
#include <QString>
#include <QApplication>
#include <QPixmap>

#include "global.h"
#include "listboxlabeledpict.h"

// ------------------------------------------------------------------------------------------------
//                                     Static data
// ------------------------------------------------------------------------------------------------

const int ListBoxLabeledPict::RTTI = 10025;

// -------------------------------------------------------------------------------------------------


/*!
    \class ListBoxLabeledPict

    \brief The ListBoxLabeledPict class provides list box items with a pixmap and text.

    This class provides a QListBoxItem with a (big) picture and some text as
    usual in KDE configuration dialogs.

    \ingroup gui

    \sa QListBox, QListBoxItem
*/

/*!
    Creates a new instance of a ListBoxLabeledPict.
    \param listbox the listbox
    \param pixmap the pixmap
    \param string the string that is displayed
*/
ListBoxLabeledPict::ListBoxLabeledPict(Q3ListBox      *listbox,
                                       const QPixmap  &pixmap,
                                       const QString  &string)
    : Q3ListBoxItem(listbox)
    , m_pixmap(pixmap)
{
    setText(string);
}


/*!
    Creates a new instance of a ListBoxLabeledPict.
    \param pixmap the pixmap
    \param text the text
*/
ListBoxLabeledPict::ListBoxLabeledPict(const QPixmap& pixmap, const QString& text)
    : m_pixmap(pixmap)
{
    setText(text);
}


/*!
    Creates a new instance of a ListBoxLabeledPict.
    \param listbox the listbox to insert
    \param pixmap the pixmap
    \param text the text
    \param after the iterm after which this new item should be inserted
*/
ListBoxLabeledPict::ListBoxLabeledPict(Q3ListBox* listbox, const QPixmap& pixmap,
            const QString& text, Q3ListBoxItem *after)
    : Q3ListBoxItem(listbox, after), m_pixmap(pixmap)
{
    setText(text);
}


/*!
    Returns a pointer to the pixmap hold by this class.
    \return the pointer which becames invalid after deleting this item
*/
const QPixmap* ListBoxLabeledPict::pixmap() const
{
    return &m_pixmap;
}

/*!
    Draws the pixmap using \a painter.
*/
void ListBoxLabeledPict::paint( QPainter *painter )
{
    int itemWidth = listBox()->maxItemWidth();
    int xPos, yPos;

    const QPixmap* pm = pixmap();
    if ( pm && ! pm->isNull() )
    {
        xPos = ( itemWidth - pm->width() ) / 2;
        painter->drawPixmap( xPos, 4, *pm);
    }


    QFontMetrics fm = painter->fontMetrics();
    xPos = ( itemWidth - fm.width(text()) ) / 2;
    yPos = 5 + fm.height() + pm->width();
    painter->drawText( xPos, yPos, text() );

}

/*!
    Returns the height of the pixmap in list box \a lb.

    \sa paint(), width()
*/

int ListBoxLabeledPict::height( const Q3ListBox* lb ) const
{
    int h = m_pixmap.height() + lb->fontMetrics().lineSpacing() + 12;
    return QMAX( h, QApplication::globalStrut().height() );
}

/*!
    Returns the width of the pixmap plus some margin in list box \a lb.

    \sa paint(), height()
*/

int ListBoxLabeledPict::width( const Q3ListBox* lb ) const
{
    return QMAX( QMAX( m_pixmap.width(), lb->fontMetrics().width( text() ) + 6),
	    QApplication::globalStrut().width() );
}


/*!
    Returns 10025.
    \return 10025
*/
int ListBoxLabeledPict::rtti() const
{
    return RTTI;
}

// :maxLineLen=100:
