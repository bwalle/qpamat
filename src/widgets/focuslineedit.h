/*
 * Id: $Id: focuslineedit.h,v 1.2 2003/12/29 00:37:11 bwalle Exp $
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
#ifndef FOCUSLINEEDIT_H
#define FOCUSLINEEDIT_H

#include <qlineedit.h>
#include <qevent.h>
#include <qstring.h>
#include <qwidget.h>

class FocusLineEdit : public QLineEdit
{
    Q_OBJECT
    
    public:
        FocusLineEdit(QWidget* parent, const char* name = 0);
        FocusLineEdit(const QString& contents, QWidget* parent, const char* name = 0);
        FocusLineEdit(const QString& contents, const QString& inputMask, QWidget* parent,
             const char* name = 0);
             
     protected:
        void focusInEvent(QFocusEvent* evt);
        
     signals:
        void gotFocus();
};

#endif // FOCUSLINEEDIT_H
