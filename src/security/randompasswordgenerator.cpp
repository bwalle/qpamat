/*
 * Id: $Id: randompasswordgenerator.cpp,v 1.8 2003/12/18 22:00:12 bwalle Exp $
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

#include <openssl/rand.h>

#include "../global.h"
#include "encodinghelper.h"
#include "randompasswordgenerator.h"

// -------------------------------------------------------------------------------------------------
bool RandomPasswordGenerator::isSeeded()
// -------------------------------------------------------------------------------------------------
{
    return RAND_status() == 1;
}


// -------------------------------------------------------------------------------------------------
QString RandomPasswordGenerator::getPassword(uint length, QCharVector allowedChars) 
// -------------------------------------------------------------------------------------------------
        throw (PasswordGenerateException)
{
    QString ret;
    byte buffer[1];
    
    while (ret.length() < length)
    {
        if (RAND_bytes(buffer, 1) == 0)
        {
            throw PasswordGenerateException("The object was not seeded so I cannot generate a "
                "random password");
        }
        
        QChar c(buffer[0]);
        
        if (qFind(allowedChars.begin(), allowedChars.end(), c) != allowedChars.end())
        {
            ret += c;
        }
    }
    
    return ret;
}


// -------------------------------------------------------------------------------------------------
bool RandomPasswordGenerator::isSlow() const
// -------------------------------------------------------------------------------------------------
{
    return false;
}

