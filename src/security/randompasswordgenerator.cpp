/*
 * Id: $Id: randompasswordgenerator.cpp,v 1.5 2003/12/10 21:48:26 bwalle Exp $
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
#include "notseededexception.h"
#include "encodinghelper.h"
#include "randompasswordgenerator.h"

// -------------------------------------------------------------------------------------------------
bool RandomPasswordGenerator::isSeeded()
// -------------------------------------------------------------------------------------------------
{
    return RAND_status() == 1;
}


// -------------------------------------------------------------------------------------------------
QString RandomPasswordGenerator::getPassword(int length) throw (NotSeededException)
// -------------------------------------------------------------------------------------------------
{
    byte* buffer = new byte[length];
    if (RAND_bytes(buffer, length) == 0)
    {
        throw NotSeededException("The object was not seeded so I cannot generate a random "\
            "password");
    }
    ByteVector vector(length);
    qCopy(buffer, buffer+length, vector.begin());
    QString base64 = EncodingHelper::toBase64(vector);
    delete[] buffer;
    base64.truncate(length);
    return base64;
}


// -------------------------------------------------------------------------------------------------
RandomPasswordGenerator& RandomPasswordGenerator::getInstance()
// -------------------------------------------------------------------------------------------------
{
    static RandomPasswordGenerator theInstance;
    return theInstance;
}

