/*
 * Id: $Id: encryptor.h,v 1.10 2003/12/10 21:47:47 bwalle Exp $
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

#include <qstring.h>
#include <qstringlist.h>

#include "stringencryptor.h"
#include "nosuchalgorithmexception.h"
#include "../global.h"

/*!
 * \brief An interface for an object that encrypts bytes.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.10 $
 * \date $Date: 2003/12/10 21:47:47 $
 */
class Encryptor : public StringEncryptor
{
    public:
        
        /*!
         * Encrypts the given amount of bytes.
         * \param vector the bytes to encrypt
         * \return the encrypted bytes
         */
        virtual ByteVector encrypt(const ByteVector& vector) = 0;
        
        /*!
         * Encrypts the given string.
         * \param string string to enctrypt
         * \return the encrypted bytes
         */
        virtual ByteVector encryptStrToBytes(const QString& string) = 0;
        
        /*!
         * Decrypts the given amount of bytes.
         * \param vector the bytes to decrypt
         * \return the decrypted bytes
         */
        virtual ByteVector decrypt(const ByteVector& vector) = 0;
        
        /*!
         * Decrypts the given amount of bytes. This method returns a String. 
         * Call this method only if you are sure that vector is the result of
         * an encryption with encryptStrToBytes().
         * \param vector the bytes to decrypt
         * \return the decrypted string
         */
        virtual QString decryptStrFromBytes(const ByteVector& vector) = 0;
};

#endif // ENCRYPTOR_H
