/*
 * Id: $Id: focuslineedit.cpp,v 1.2 2003/12/29 00:37:11 bwalle Exp $
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
#include <qstring.h>
#include <qlineedit.h>
#include <qobject.h>

#include "focuslineedit.h"


/*!
    \class FocusLineEdit
    
    \brief Implements a QLineEdit which notifies the user when it gets the focus.

    This FocusLineEdit is exactly the same as a normal QLineEdit but it has an additional signal
    gotFocus() which notifies the user of this widget to do something if the element got the focus.
    
    \ingroup widgets
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date: 2003/12/29 00:37:11 $
*/

/*!
    \fn FocusLineEdit::gotFocus()
    
    This signal is emitted if the widget got the focus.
*/

/*!
    Constructor passed to QLineEdit.
    \param parent the parent
    \param name the name
*/
FocusLineEdit::FocusLineEdit(QWidget* parent, const char* name)
    : QLineEdit(parent, name)
{}


/*!
    Constructor passed to QLineEdit.
    \param contents initial content
    \param parent the parent
    \param name the name
*/
FocusLineEdit::FocusLineEdit(const QString& contents, QWidget* parent, const char* name)
    : QLineEdit(contents, parent, name)
{}


/*!
    Constructor passed to QLineEdit.
    \param contents initial content
    \param inputMask the input mask
    \param parent the parent
    \param name the name
*/
FocusLineEdit::FocusLineEdit(const QString& contents, const QString& inputMask, QWidget* parent, 
            const char* name)
    : QLineEdit(contents, inputMask, parent, name)
{}


/*!
    Event that happens if the line edit gets the focus. It just calls the QLineEdit::focusInEvent()
    and emits a gotFocus() signal.
    \param evt the event
*/
void FocusLineEdit::focusInEvent(QFocusEvent* evt)
{
    QLineEdit::focusInEvent(evt);
    emit gotFocus();
}
