/*
 * Id: $Id: stringencryptor.h,v 1.2 2003/12/29 10:59:16 bwalle Exp $
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
#ifndef STRINGENCRYPTOR_H
#define STRINGENCRYPTOR_H

#include <stdexcept>

#include <qstring.h>
#include <qstringlist.h>

#include "global.h"

class NoSuchAlgorithmException : public std::runtime_error
{
    public:
        NoSuchAlgorithmException(const std::string& error) : std::runtime_error(error) { }
};

class StringEncryptor
{
    public:
        virtual ~StringEncryptor() { }
        
        virtual QString encryptStrToStr(const QString& string) = 0;
        virtual QString decryptStrFromStr(const QString& string) = 0;
};

#endif // STRINGENCRYPTOR_H
