/*
 * Id: $Id: configpasswordchecker.cpp,v 1.2 2003/12/21 20:31:51 bwalle Exp $
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

#include "../global.h"
#include "configpasswordchecker.h"

// -------------------------------------------------------------------------------------------------
ConfigPasswordChecker::ConfigPasswordChecker(int length, const QString& ensuredChars, 
    const QString& allowedChars)
// -------------------------------------------------------------------------------------------------
        : m_ensuredChars(ensuredChars), m_length(length)
{
    // process allowed chars
    QString allowed = allowedChars;
    allowed = allowed.replace("A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXZY")
                 .replace("a-z", "abcdefghijklmnopqrstuvwxzy").replace("0-9", "0123456789");
    m_allowedChars.resize(allowed.length());
    for (uint i = 0; i < allowed.length(); ++i)
    {
        m_allowedChars[i] = allowed[i];
    }
}


// -------------------------------------------------------------------------------------------------
bool ConfigPasswordChecker::isPasswordOk(const QString& password) throw (PasswordCheckException)
// -------------------------------------------------------------------------------------------------
{
    if (password.length() < minimalLength())
    {
        return false;
    }
    
    // if we don't need this, we just set it to true here initially
    bool uppercase = !m_ensuredChars.contains("U"); 
    bool lowercase = !m_ensuredChars.contains("L");
    bool digit = !m_ensuredChars.contains("D");;
    bool special = !m_ensuredChars.contains("S");;
    
    uint numOfPairs = 0;
    for (uint i = 0; i < password.length(); ++i)
    {
        const QChar character = password[i];
        QChar::Category cat = character.category();
        if (cat == QChar::Letter_Uppercase)
        {
            uppercase = true;
        } 
        else if (cat == QChar::Letter_Lowercase)
        {
            lowercase = true;
        }
        else if (character.isDigit())
        {
            digit = true;
        }
        else if (! character.isLetterOrNumber())
        {
            special = true;
        }
        
        if (password.contains(character) > 3)
        {
            ++numOfPairs;
        }
    }
    
    if (!(uppercase && lowercase && digit && special && numOfPairs <= (password.length() / 4)))
    {
        return false;
    }
    
    if (!m_allowedChars.isEmpty())
    {
        for (uint i = 0; i < password.length(); ++i)
        {
            QChar current = password[i];
            if (qFind( m_allowedChars.begin(), m_allowedChars.end(), current ) 
                    == m_allowedChars.end())
            {
                return false;
            }
        }
    }
    
    return true;
}


// -------------------------------------------------------------------------------------------------
uint ConfigPasswordChecker::minimalLength() const
// -------------------------------------------------------------------------------------------------
{
    return m_length;
}


// -------------------------------------------------------------------------------------------------
QCharVector ConfigPasswordChecker::allowedCharacters() const
// -------------------------------------------------------------------------------------------------
{
    return m_allowedChars;
}
