/*
 * Id: $Id: randompasswordgenerator.cpp,v 1.9 2003/12/29 10:59:16 bwalle Exp $
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

#include "global.h"
#include "encodinghelper.h"
#include "randompasswordgenerator.h"


/*!
    \class RandomPasswordGenerator
    
    \brief Object to generate random passwords. 

    The passwords are \b really random since it is used the OpenSSL library for randomness.
    
    \ingroup security
    \author Bernhard Walle
    \version $Revision: 1.9 $
    \date $Date: 2003/12/29 10:59:16 $
*/

/*!
    Generates a random password. There's a random password generated using a
    function of the OpenSSL library. This password consists of bytes. The
    bytes are transformed to a string using the base64 algorithm, which
    ensures that there could be no charset conversion problem.
    The passwords are not easy to memorize but you have QPaMaT and your clipboard.
    However, if you need a good password to memorize, just use your brain. A
    computer cannot output what is easy to memorize for <b>you</b>!
    \param length the length of the password
    \param pAllowedChars list of all allowed characters (see PasswordGenerator::getPassword())
    \return the password
    \exception PasswordGenerateException if the object was not seeded
*/
QString RandomPasswordGenerator::getPassword(uint length, const QString& pAllowedChars) 
        throw (PasswordGenerateException)
{
    QString ret;
    byte buffer[1];
    QString allowedChars = pAllowedChars;
    
    bool allAllowed = allowedChars.isNull();
    
    // build the list of allowed characters
    if (!allAllowed)
    {
        allowedChars.replace("a-z", "abcdefghijklmnopqrstuvwxzy");
        allowedChars.replace("A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXZY");
        allowedChars.replace("0-9", "0123456789");
    }
    
    while (ret.length() < length)
    {
        if (RAND_bytes(buffer, 1) == 0)
        {
            throw PasswordGenerateException("The object was not seeded so I cannot generate a "
                "random password");
        }
        char c = char(buffer[0]);
        
        if (allowedChars.contains(c))
        {
            ret += c;
        }
    }
    
    return ret;
}


/*!
    Returns if the object was seeded. On Linux or other operating systems with
    a /dev/urandom device this is done automatically, on other system this
    must be done with user input. (System time or something else is not
    enouth.)
*/
bool RandomPasswordGenerator::isSeeded()
{
    return RAND_status() == 1;
}


/*!
    Returns \c false
    \return \c false
*/
bool RandomPasswordGenerator::isSlow() const
{
    return false;
}

