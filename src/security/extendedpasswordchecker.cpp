/*
 * Id: $Id: extendedpasswordchecker.cpp,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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

#include "extendedpasswordchecker.h"

// -------------------------------------------------------------------------------------------------
bool ExtendedPasswordChecker::isPasswordOk(const QString& password) throw (PasswordCheckException)
// -------------------------------------------------------------------------------------------------
{
    if (password.length() < minimalLength())
    {
        return false;
    }
    
    bool uppercase = false;
    bool lowercase = false;
    bool digit = false;
    bool special = false;
    int numOfPairs;
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
        
        if (password.contains(character) > 1)
        {
            ++numOfPairs;
        }
    }
    
    return uppercase && lowercase && digit && special && numOfPairs <= 1;
}


// -------------------------------------------------------------------------------------------------
uint ExtendedPasswordChecker::minimalLength()
// -------------------------------------------------------------------------------------------------
{
    return 8;
}

