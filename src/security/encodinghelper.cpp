/*
 * Id: $Id: encodinghelper.cpp,v 1.1 2003/09/17 21:28:01 bwalle Exp $
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
#include <stdexcept>
 
#include <qstring.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include "encodinghelper.h"

// -------------------------------------------------------------------------------------------------
const char EncodingHelper::base64Alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',     // 00 .. 07
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',     // 08 .. 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',     // 16 .. 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',     // 24 .. 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',     // 32 .. 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',     // 40 .. 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',     // 48 .. 55
    '4', '5', '6', '7', '8', '9', '+', '/'  };  // 56 .. 63

const char EncodingHelper::reverseBase64Alphabet[] = {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
     -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51                       };

// -------------------------------------------------------------------------------------------------
QString EncodingHelper::toBase64(const QValueVector<unsigned char>& array)
{
    QString result;
    unsigned char a, b, c;
    int lenMod3;
    
    for (unsigned int i = 0; i < array.size(); i += 3)
    {
        lenMod3 = ((i+3) > array.size()) ? (array.size() % 3) : 3;
        a = array[i+0];
        b = lenMod3 > 1 ? array[i+1] : 0;
        c = lenMod3 > 2 ? array[i+2] : 0;
        result += base64Alphabet[a >> 2];
        result += base64Alphabet[((a << 4) & 0x30) | (b >> 4)];
        result += (lenMod3 > 1) ? base64Alphabet[((b << 2) & 0x3C) | ((c >> 6) & 0x03)] : '=';
        result += (lenMod3 > 2) ? base64Alphabet[c & 0x3F] : '=';
    }
    
    return result;
}


// -------------------------------------------------------------------------------------------------
QValueVector<unsigned char> EncodingHelper::fromBase64(const QString& string)
{
    int stringLength = string.length();
    if (stringLength % 4 != 0)
    {
        throw invalid_argument("In EncodingHelper::fromBase64: string % 4 != 0");
    }
    const char* stringAscii = string.ascii();
    QValueVector<unsigned char> vector;
    char a, b, c, d;
    for (int i = 0; i < stringLength; i += 4)
    {
        a = reverseBase64Alphabet[ stringAscii[i] ];
        b = reverseBase64Alphabet[ stringAscii[i+1] ];
        c = reverseBase64Alphabet[ stringAscii[i+2] ];
        d = reverseBase64Alphabet[ stringAscii[i+3] ];
        
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


