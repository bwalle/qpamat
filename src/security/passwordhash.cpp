/*
 * Id: $Id: passwordhash.cpp,v 1.1 2003/10/11 19:51:50 bwalle Exp $
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
const int PasswordHash::numberOfRandomBits = 8;
// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------
bool PasswordHash::isCorrect(QString password, QString hash)
// -------------------------------------------------------------------------------------------------
{
    ByteVector output(numberOfRandomBits);
    ByteVector input = EncodingHelper::fromBase64(hash);
#ifdef QT_RANGE_CHECK
    Q_ASSERT(input.size() > numberOfRandomBits);
#endif

    for (int i = 0; i < numberOfRandomBits; ++i)
    {
        password.prepend(input[i]);
        output[i] = input[i];
    }
    attachHashWithoutSalt(output, password);
    return output == input;
}


// -------------------------------------------------------------------------------------------------
QString PasswordHash::generateHash(QString password)
// -------------------------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------------------------
void PasswordHash::attachHashWithoutSalt(ByteVector& output, const QString& password)
// -------------------------------------------------------------------------------------------------
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

