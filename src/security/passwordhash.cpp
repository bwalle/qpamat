/*
 * Id: $Id: passwordhash.cpp,v 1.2 2003/12/29 10:59:16 bwalle Exp $
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
#include <cstdlib>

#include <openssl/evp.h>

#include <qstring.h>
#include <qcstring.h>

#include "constants.h"
#include "passwordhash.h"
#include "encodinghelper.h"

// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

const int PasswordHash::numberOfRandomBits = 8;

/*!
    \class PasswordHash
    
    \brief Helping functions for dealing with passwords and hashes.
    \ingroup security
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date: 2003/12/29 10:59:16 $
*/

/*!
    Checks the given password if it is the same as the one stored in the hash.
    (There's no way to re-construct the password from the hash. There's a hash
    generated from the password and this one is compared to that which was
    provided by \p hash.)
    \param password the password
    \param hash the hash object
    \return \c true if the password is correct, \c false otherwise
*/
bool PasswordHash::isCorrect(QString password, QString hash)
{
    ByteVector output(numberOfRandomBits);
    ByteVector input = EncodingHelper::fromBase64(hash);
    
    Q_ASSERT(input.size() > uint(numberOfRandomBits));
    
    for (int i = 0; i < numberOfRandomBits; ++i)
    {
        password.prepend(input[i]);
        output[i] = input[i];
    }
    attachHashWithoutSalt(output, password);
    return output == input;
}


/*!
    Generates a hash from the password. That hash contains a salt, too, which
    makes storing and veryfiing hashes more secure. To be more precisely, a
    so-called dictinoary attack is prepended which this method because the
    dictionary would be very large.
*/
QString PasswordHash::generateHash(QString password)
{
    ByteVector output(numberOfRandomBits);
    
    srand( (uint)time(0) );
    
    for (int i = 0; i < numberOfRandomBits; ++i)
    {
        output[i] = byte( ( double(rand())/RAND_MAX)*256 );
        password.prepend(output[i]);
    }
    attachHashWithoutSalt(output, password);
    
    return EncodingHelper::toBase64(output);
}


/*!
    Attaches hash without salt
    \param output the output
    \param password the password
    \todo Find out what the function really does
*/
void PasswordHash::attachHashWithoutSalt(ByteVector& output, const QString& password)
{
    EVP_MD_CTX mdctx;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    uint md_len;
    uint oldLen = output.size();
    QCString pwUtf8 = password.utf8();
    
    EVP_DigestInit(&mdctx, HASH_ALGORITHM);
    EVP_DigestUpdate(&mdctx, (byte*)((const char*)(pwUtf8)), pwUtf8.length());
    EVP_DigestFinal(&mdctx, md_value, &md_len);
    
    output.resize(oldLen + md_len);
    qCopy(md_value, md_value + md_len, output.begin() + oldLen);
}

