/*
 * Id: $Id: collectencryptor.h,v 1.4 2004/01/13 23:20:17 bwalle Exp $
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
#ifndef COLLECTENCRYPTOR_H
#define COLLECTENCRYPTOR_H

#include <stdexcept>

#include <qstring.h>
#include <qstringlist.h>

#include "global.h"
#include "encryptor.h"

class CollectEncryptor : public StringEncryptor
{
    public:
        CollectEncryptor(Encryptor& encryptor);
        
        QString encryptStrToStr(const QString& string);
        QString decryptStrFromStr(const QString& string) 
            throw (std::invalid_argument, std::range_error);
        
        void setBytes(const ByteVector& vector);
        ByteVector getBytes();
        
    private:
        Encryptor&  m_realEncryptor;
        ByteVector  m_bytes;
};

#endif // COLLECTENCRYPTOR_H
