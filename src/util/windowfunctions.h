/*
 * Id: $Id: windowfunctions.h,v 1.1 2005/02/15 02:02:02 bwalle Exp $
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
#ifndef WINDOWFUNCTIONS_H
#define WINDOWFUNCTIONS_H

#include <qwidget.h>

class WindowFunctions
{
    public:
        static void bringToFront(QWidget* w);
        
};

#endif /* WINDOWFUNCTIONS_H */
