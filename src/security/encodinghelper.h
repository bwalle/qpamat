/*
 * Id: $Id$
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
#ifndef ENCODINGHELPER_H
#define ENCODINGHELPER_H

#include <iostream>

#include <Q3MemArray>
#include <QString>
#include <Q3ValueVector>

#include "global.h"

class EncodingHelper
{
    public:
        static QString toBase64(const ByteVector& vector);
        static ByteVector fromBase64(const QString& string);
        
        static const char base64Alphabet[];
        static const char reverseBase64Alphabet[];
    
    private:
        EncodingHelper() {}
        
};

#endif // ENCODINGHELPER_H
