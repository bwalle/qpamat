/*
 * Id: $Id: cursor.cpp,v 1.1 2004/01/08 23:40:08 bwalle Exp $
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
#include "cursor.h"

/*!
    \class Cursor 
    
    \brief Provides different cursors
    
    This class provides different cursor-styles that Qt does not include. At the moment,
    there's only one Windows-like hand cursor from KDE.
    
    \ingroup widgets
    \author $Author: bwalle $
    \version $Revision: 1.1 $
    \date $Date: 2004/01/08 23:40:08 $
*/

/*!
    Used to get a Cursor like KDE's hand cursor which has the standard Windows (Internet Explorer
    etc.) look, i.e. white with black borders. Makes a QListBox with ListBoxLabeledPict elements
    more KDE-like.
    \return the cursor
*/
QCursor Cursor::handCursorWindows()
{
    static QCursor *hand_cursor = 0;

    if (hand_cursor == 0)
    {
        static const unsigned char HAND_BITS[] = 
        {
            0x80, 0x01, 0x00, 0x40, 0x02, 0x00, 0x40, 0x02, 0x00, 0x40, 0x02,
            0x00, 0x40, 0x02, 0x00, 0x40, 0x02, 0x00, 0x40, 0x1e, 0x00, 0x40,
            0xf2, 0x00, 0x40, 0x92, 0x01, 0x70, 0x92, 0x02, 0x50, 0x92, 0x04,
            0x48, 0x80, 0x04, 0x48, 0x00, 0x04, 0x48, 0x00, 0x04, 0x08, 0x00,
            0x04, 0x08, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x20,
            0x00, 0x02, 0x40, 0x00, 0x02, 0x40, 0x00, 0x01, 0xc0, 0xff, 0x01
        };
        static const unsigned char HAND_MASK_BITS[] = 
        {
            0x80, 0x01, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03,
            0x00, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xc0, 0x1f, 0x00, 0xc0,
            0xff, 0x00, 0xc0, 0xff, 0x01, 0xf0, 0xff, 0x03, 0xf0, 0xff, 0x07,
            0xf8, 0xff, 0x07, 0xf8, 0xff, 0x07, 0xf8, 0xff, 0x07, 0xf8, 0xff,
            0x07, 0xf8, 0xff, 0x07, 0xf0, 0xff, 0x07, 0xf0, 0xff, 0x07, 0xe0,
            0xff, 0x03, 0xc0, 0xff, 0x03, 0xc0, 0xff, 0x01, 0xc0, 0xff, 0x01
        };
        QBitmap hand_bitmap(22, 22, HAND_BITS, true);
        QBitmap hand_mask(22, 22, HAND_MASK_BITS, true);
        hand_cursor = new QCursor(hand_bitmap, hand_mask, 7, 0);
        // Hack to force QCursor to call XCreatePixmapCursor() immediately
        // so the bitmaps don't get pushed out of the Xcursor LRU cache.
        hand_cursor->handle();
    }

    Q_CHECK_PTR(hand_cursor);
    return *hand_cursor;
}

