/*
 * Id: $Id: stringencryptor.h,v 1.1 2003/12/10 21:48:05 bwalle Exp $
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

#include <qstring.h>
#include <qstringlist.h>

#include "nosuchalgorithmexception.h"
#include "../global.h"

/*!
 * \brief An interface for an object that encrypts bytes.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/10 21:48:05 $
 */
class StringEncryptor
{
    public:
        
        /*!
         * Destroys a StringEncryptor object.
         */
        virtual ~StringEncryptor() { }
        
        /*!
         * Encrypts the given string and returns a base 64 encoded string.
         * \param string the string to encrypt
         * \return the encrypted bytes
         */
        virtual QString encryptStrToStr(const QString& string) = 0;
        
        /*!
         * Decrypts the given Base 64 string. Call this method only if you are sure that
         * the string is a result of Encryptor::encryptBase64()!
         * \param string the encryted Base-64-encoded string
         * \return the decrypted string
         */
        virtual QString decryptStrFromStr(const QString& string) = 0;
};

#endif // STRINGENCRYPTOR_H
