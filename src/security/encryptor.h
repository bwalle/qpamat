/*
 * Id: $Id: encryptor.h,v 1.1 2003/09/21 16:01:35 bwalle Exp $
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
#include <qstring.h>

#include <openssl/evp.h>

#include "nosuchalgorithmexception.h"
#include "../types.h"

/**
 * A object which encrypts bytes.
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/09/21 16:01:35 $
 */
class Encryptor
{
    public:
        /**              
         * Creates an instance of a new Encryptor for the given algorithm.
         * Following algorithms may be supported:
         * 
         *  - Blowfish (\c BLOWFISH)
         *  - International Data Encryption Algorithm (\c IDEA)
         *  - CAST (\c CAST)
         *  - Triple-Data Encryption Standard (\c 3DES)
         *  - Advances Encryption Standard (\c AES)
         *
         * Which algorithms are available in reality depends on OpenSSL. It can only be
         * checked at runtime. You cat a list of available algorithms using the
         * getAlgorithms() function in this class.
         *
         * @param algorithm the algorithm as string
         * @param password The password for encryption and decryption.
         * @exception NoSuchAlgorithmException if the algorithm is not supported
         */
        Encryptor(const QString& algorithm, const QString& password)
            throw (NoSuchAlgorithmException);
        
        /**
         * Encrypts the given amount of bytes.
         * @param vector the bytes to encrypt
         * @return the encrypted bytes
         */
        virtual ByteVector encrypt(const ByteVector& vector);
        
        /**
         * Encrypts the given string.
         * @param string string to enctrypt
         * @return the encrypted bytes
         */
        virtual ByteVector encryptString(const QString& string);
        
        /**
         * Decrypts the given amount of bytes.
         * @param vector the bytes to decrypt
         * @return the decrypted bytes
         */
        virtual ByteVector decrypt(const ByteVector& vector);
        
        /**
         * Decrypts the given amount of bytes. This method returns a String. 
         * Call this method only if you are sure that vector is the result of
         * an encryption with encryptString().
         * @param vector the bytes to decrypt
         * @return the decrypted string
         */
        virtual QString decryptString(const ByteVector& vector);
        
        /**
         * Sets a new a new password.
         * @param password the password
         */
        virtual void setPassword(const QString& password);
        
        /**
         * Returns a list of all available cipher algorithms. It depends on the
         * OpenSSL configuration.
         * @return the list
         * @see Encryptor()
         */
        static QStringList getAlgorithms();
        
        /**
         * Returns the default algorithm used for new files QPaMaT. As my personal
         * preference I suggest Blowfish as default algorithm if available. It's fast,
         * not protected by patents and is held for secure.
         * @return the name of the algorithm
         */
        static QString getSuggestedAlgorithm();
        
        /**
         * Deletes an Encryptor object. Nothing to do here but maybe needed for
         * subclassing.
         */
        virtual ~Encryptor();
        
    protected:
        /**
         * Represents the operation to perform. Needed for the crypt() function.
         */
        enum OperationType { DECRYPT /**< decryption */, ENCRYPT /**< encryption */};
        
        /**
         * Encrypts or decrypts a message.
         * @param vector the vector to encrypt/decrypt
         * @param operation the operation (encrypt or decrypt)
         * @return the crypted bytes
         */
        virtual ByteVector crypt(const ByteVector& vector, OperationType operation);
        
    private:
        const EVP_CIPHER*     m_cipher_algorithm;
        byte                  m_key[EVP_MAX_KEY_LENGTH];
        byte                  m_iv[EVP_MAX_IV_LENGTH];
        
        static StringMap initAlgorithmsMap();
        static StringMap m_algorithms;
        
        
};


