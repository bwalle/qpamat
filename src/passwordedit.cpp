/*
 * Id: $Id: passwordedit.cpp,v 1.1 2003/09/14 17:10:31 bwalle Exp $
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
#include <qobject.h>
#include <qwidget.h>
#include <qfont.h>

#include "passwordedit.h"

// -------------------------------------------------------------------------------------------------
PasswordEdit::PasswordEdit(QWidget* parent, const char* name)
        : QLineEdit(parent, name)
{   
    init();
}


// -------------------------------------------------------------------------------------------------
PasswordEdit::PasswordEdit(const QString& content, QWidget* parent, const char* name)
        : QLineEdit(content, parent, name)
{
    init();
}


// -------------------------------------------------------------------------------------------------
void PasswordEdit::setFillCharacter(const QChar& character)
{
    m_fillCharacter = character;
}


// -------------------------------------------------------------------------------------------------
QChar PasswordEdit::fillCharacter() const
{
    return m_fillCharacter;
}


// -------------------------------------------------------------------------------------------------
void PasswordEdit::init()
{
    setFillCharacter('*');
}


// -------------------------------------------------------------------------------------------------
void PasswordEdit::keyPressEvent (QKeyEvent* evt)
{
    QString entered = evt->text();

    switch(evt->key())
    {


    }
    if (entered.latin1()[0] < ' ') // control character, according to ASCII which is part
                        // of Unicode
    {
        QLineEdit::keyPressEvent(evt);
        //m_password getText()
    }
    else
    {
        m_password += evt->ascii();
        m_displayedPassword += entered;
    
        if (m_displayedPassword.length() >= 2)
        {
            m_displayedPassword[m_displayedPassword.length()-2] = fillCharacter();
        }
        setText(m_displayedPassword);
    }
}

