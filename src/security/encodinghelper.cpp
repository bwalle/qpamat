/*
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
#include <stdexcept>
 
#include <QString>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include "encodinghelper.h"

// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

/*!
    The Base 64 alphabet from 0 to 63 described in RFC 2045 
    (http://www.faqs.org/rfcs/rfc2045.html). The index is the number and
    the result is the corresponding character.
*/
const char EncodingHelper::base64Alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',     // 00 .. 07
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',     // 08 .. 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',     // 16 .. 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',     // 24 .. 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',     // 32 .. 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',     // 40 .. 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',     // 48 .. 55
    '4', '5', '6', '7', '8', '9', '+', '/'  };  // 56 .. 63

/*!
    The reverse Base 64 alphabet, i.e. the index is the character and the result is
    the corresponding number.
*/
const char EncodingHelper::reverseBase64Alphabet[] = {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
     -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51                       };


/*!
    \class EncodingHelper
    
    \brief Helper class for dealing with encodings.
    
    \ingroup security
    \author Bernhard Walle
*/

/*!
    Converts the bytes that are in the given vector.
    \param vector the vector with the bytes
    \return the string
*/
QString EncodingHelper::toBase64(const ByteVector& vector)
{
    QString result;
    unsigned char a, b, c;
    int lenMod3;
    
    for (int i = 0; i < vector.size(); i += 3)
    {
        lenMod3 = ((i+3) > vector.size()) ? (vector.size() % 3) : 3;
        a = vector[i];
        b = lenMod3 > 1 ? vector[i+1] : 0;
        c = lenMod3 > 2 ? vector[i+2] : 0;
        result += base64Alphabet[a >> 2];
        result += base64Alphabet[((a << 4) & 0x30) | (b >> 4)];
        result += (lenMod3 > 1) ? base64Alphabet[((b << 2) & 0x3C) | ((c >> 6) & 0x03)] : '=';
        result += (lenMod3 > 2) ? base64Alphabet[c & 0x3F] : '=';
    }
    return result;
}


/*!
    Converts the base64 encoded string to the original bytes. The length of
    the string must be correct, i.e. dividable by 4.
    \param string the encoded string
    \return the decoded bytes
    \exception std::invalid_argument if the length is incorrect
*/
ByteVector EncodingHelper::fromBase64(const QString& string)
{
    int stringLength = string.length();
    if (stringLength % 4 != 0)
    {
        throw std::invalid_argument("In EncodingHelper::fromBase64: string % 4 != 0");
    }
    const char* stringAscii = string.ascii();
    Q3ValueVector<unsigned char> vector;
    char a, b, c, d;
    for (int i = 0; i < stringLength; i += 4)
    {
        a = reverseBase64Alphabet[ (int)stringAscii[i] ];
        b = reverseBase64Alphabet[ (int)stringAscii[i+1] ];
        c = reverseBase64Alphabet[ (int)stringAscii[i+2] ];
        d = reverseBase64Alphabet[ (int)stringAscii[i+3] ];
        
        vector.push_back(((a << 2) | (b >> 4)));
        vector.push_back(((b & 0x0F) << 4) | (c >> 2));
        vector.push_back((((c & 0x03) << 6) | d));
        
        if (d == -2)
        {
            vector.pop_back();
            if (c == -2)
            {
                vector.pop_back();
            }
        }
    }
    
    return vector;
}

// :maxLineLen=100:
