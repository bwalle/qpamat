/*
 * Id: $Id: encryptor.h,v 1.11 2003/12/29 10:59:16 bwalle Exp $
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
#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <stdexcept>

#include <qstring.h>
#include <qstringlist.h>

#include "global.h"
#include "stringencryptor.h"

class Encryptor : public StringEncryptor
{
    public:
        virtual ~Encryptor() {};
        
        virtual ByteVector encrypt(const ByteVector& vector) = 0;
        virtual ByteVector encryptStrToBytes(const QString& string) = 0;
        
        virtual ByteVector decrypt(const ByteVector& vector) = 0;
        virtual QString decryptStrFromBytes(const ByteVector& vector) = 0;
};

#endif // ENCRYPTOR_H
