/*
 * Id: $Id: encodinghelper.h,v 1.2 2003/09/20 13:38:41 bwalle Exp $
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
#include <qmemarray.h>
#include <qstring.h>
#include <qvaluevector.h>

#include "types.h"

/**
 * @file encodinghelper.h
 * This file contains helping functions for Encoding and Decoding of binary data.
 */

// =================================================================================================

/**
 * Helper class for dealing with encodings.
 * @author Bernhard Walle
 * @version $Revision: 1.2 $
 * @date $Date: 2003/09/20 13:38:41 $
 */
class EncodingHelper
{
    public:
        
        /**
         * Converts the bytes that are in the given vector.
         * @param vector the vector with the bytes
         * @return the string
         */
        static QString toBase64(const ByteVector& vector);
        
        /**
         * Converts the base64 encoded string to the original bytes. The length of
         * the string must be correct, i.e. dividable by 4.
         * @param string the encoded string
         * @return the decoded bytes
         * @exception std::invalid_argument if the length is incorrect
         */
        static ByteVector fromBase64(const QString& string);
        
        /**
         * The Base 64 alphabet from 0 to 63 described in RFC 2045 
         * (http://www.faqs.org/rfcs/rfc2045.html). The index is the number and
         * the result is the corresponding character.
         */
        static const char base64Alphabet[];
        
        /**
         * The reverse Base 64 alphabet, i.e. the index is the character and the result is
         * the corresponding number.
         */
        static const char reverseBase64Alphabet[];
    
    protected:
        
        /**
         * Allow no instantiation of the object.
         */
        EncodingHelper() {}
        
};

// =================================================================================================

/**
 * Overloads the shift operator << for a  QValueVector of any type.
 * @param ostream the output stream
 * @param vector the vector that should be "shifted"
 * @return the given output stream
 */
template<class T>
std::ostream& operator<<(std::ostream& ostream, const QValueVector<T> vector)
{
    for (QValueVector<T>::ConstIterator it = vector.begin(); it != vector.end(); ++it)
    {
        ostream << *it;
    }
    return ostream;
}

