/*
 * Id: $Id: collectencryptor.h,v 1.2 2003/12/10 21:47:47 bwalle Exp $
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

#include "nosuchalgorithmexception.h"
#include "../global.h"
#include "stringencryptor.h"
#include "encryptor.h"

/*!
 * \brief This encryptor just collects the encrypted bytes in a ByteArray.
 *
 * It is used for writing encrypted passwords to a smartcard. There are to modes of an encrytor
 * encrypting and decrypting. The real encrypting and decrypting operations are passed to the
 * real encryptor which is passed to the constructor of this object at initialization time.
 *
 * In encrypt mode the CollectEncryptor takes a string, encrypts it and appends the encrypted
 * value in the byte vector. At the beginning, the vector is empty. The return value of the
 * CollectEncryptor::encryptString() method is a string like following: \c SMARTCARD:0:27.
 * The first number after the colon indicates the offset in which the encrypted bytes are stored
 * in the byte array. The first string gets the offset \c 0, of course. The second number is
 * the length, i.e. the number of bytes. This means that the byte vector now has a length of \c 27
 * bytes.
 *
 * In decrypt mode the CollectEncryptor does the opposite. It gets a string of the form \c
 * SMARTCARD:o:l, takes the bytes according to this specification from the byte array (which must
 * be set previously!) and tries to decrypt the value.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/10 21:47:47 $
 */
class CollectEncryptor : public StringEncryptor
{
    public:
        
        /*!
         * Creates a new instance of a CollectEncryptor object.
         * \param encryptor the real encrytor used for encrypting
         */
        CollectEncryptor(Encryptor& encryptor);
        
        /*!
         * \copydoc StringEncryptor::encryptStrToStr()
         */
        QString encryptStrToStr(const QString& string);
        
        /*!
         * \copydoc StringEncryptor::decryptStrFromStr()
         * \exception std::invalid_argument if the argument is not of the specified form
         * \exception std::range_error if the byte array does not contain the specified bytes
         */
        QString decryptStrFromStr(const QString& string) 
            throw (std::invalid_argument, std::range_error);
        
        /*!
         * Sets the stored bytes.
         * \param vector the bytes
         */
        void setBytes(const ByteVector& vector);
        
        /*!
         * Gets the stored bytes.
         */
        ByteVector getBytes();
        
    private:
        Encryptor& m_realEncryptor;
        ByteVector m_bytes;
};

#endif // COLLECTENCRYPTOR_H
