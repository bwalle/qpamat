/*
 * Id: $Id: abstractencryptor.cpp,v 1.2 2003/12/10 21:47:47 bwalle Exp $
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
#include <qstringlist.h>

#include "nosuchalgorithmexception.h"
#include "../global.h"
#include "abstractencryptor.h"
#include "encodinghelper.h"


// -------------------------------------------------------------------------------------------------
ByteVector AbstractEncryptor::encryptStrToBytes(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    QCString utf8CString = string.utf8();
    uint utf8Length = utf8CString.length();
    ByteVector vector(utf8Length);
    const byte* utf8 = (const byte*)utf8CString.data();
    qCopy(utf8, utf8 + utf8Length, vector.begin());
    return encrypt(vector);
}


// -------------------------------------------------------------------------------------------------
QString AbstractEncryptor::encryptStrToStr(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    return EncodingHelper::toBase64(encryptStrToBytes(string));
}


// -------------------------------------------------------------------------------------------------
QString AbstractEncryptor::decryptStrFromBytes(const ByteVector& vector)
// -------------------------------------------------------------------------------------------------
{
    QString result;
    ByteVector decrypted = decrypt(vector);
    uint size = decrypted.size();
    char* decryptedBytes = new char[size + 1];
    decryptedBytes[size] = 0;
    qCopy(decrypted.begin(), decrypted.end(), decryptedBytes);
    
    QString returnString = QString::fromUtf8(decryptedBytes);
    delete[] decryptedBytes;
    return returnString;
}


// -------------------------------------------------------------------------------------------------
QString AbstractEncryptor::decryptStrFromStr(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    return decryptStrFromBytes(EncodingHelper::fromBase64(string));
}
