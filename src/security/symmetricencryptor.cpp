/*
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
#include <iostream>

#include <QString>
#include <QStringList>
#include <Q3CString>

#include <openssl/evp.h>
#include <openssl/ssl.h>

#include "symmetricencryptor.h"
#include "constants.h"
#include "encodinghelper.h"

#ifndef BUFLEN
#define BUFLEN 512
#endif

// -------------------------------------------------------------------------------------------------
//                                     Static members
// -------------------------------------------------------------------------------------------------

StringMap SymmetricEncryptor::m_algorithms = initAlgorithmsMap();


/*!
    \class SymmetricEncryptor

    \brief A object which encrypts bytes.

    \ingroup security
    \author Bernhard Walle
*/

/*!
    Creates an instance of a new Encryptor for the given algorithm.
    Following algorithms may be supported:

     - Blowfish (\c BLOWFISH)
     - International Data Encryption Algorithm (\c IDEA)
     - CAST (\c CAST)
     - Triple-Data Encryption Standard (\c 3DES)
     - Advances Encryption Standard (\c AES)

    Which algorithms are available in reality depends on OpenSSL. It can only be
    checked at runtime. You cat a list of available algorithms using the
    getAlgorithms() function in this class.

    \param algorithm the algorithm as string
    \param password The password for encryption and decryption.
    \exception NoSuchAlgorithmException if the algorithm is not supported
*/
SymmetricEncryptor::SymmetricEncryptor(const QString& algorithm, const QString& password)
            throw (NoSuchAlgorithmException)
{
    // set the right cipher algorithm
    if (m_algorithms.contains(algorithm.upper()))
        m_cipher_algorithm = EVP_get_cipherbyname(m_algorithms[algorithm.upper()]);
    else
        throw NoSuchAlgorithmException(("Algorithm "+algorithm+" not supported").latin1());

    // set the password
    setPassword(password);
    m_currentAlgorithm = algorithm;
}


/*!
    Returns a list of all available cipher algorithms. It depends on the OpenSSL configuration.
    \return the list
    \see Encryptor()
*/
QStringList SymmetricEncryptor::getAlgorithms()
{
    QStringList algorithms;

    for (StringMap::iterator it = m_algorithms.begin(); it != m_algorithms.end(); ++it)
        algorithms += it.key();

    return algorithms;
}


/*!
    Returns the default algorithm used for new files QPaMaT. As my personal
    preference I suggest Blowfish as default algorithm if available. It's fast,
    not protected by patents and is held for secure. AES is the same but it's not
    included in older versions of OpenSSL.
    \return the name of the algorithm
*/
QString SymmetricEncryptor::getSuggestedAlgorithm()
{
    StringVector vec;
    vec.push_back("BLOWFISH");
    vec.push_back("AES");
    vec.push_back("CAST5");

    for (StringVector::Iterator it = vec.begin(); it != vec.end(); ++it)
        if (m_algorithms.contains(*it))
            return *it;

    return QString();
}


/*!
    Initializes the algorithms map according to the OpenSSL library.
    \return StringMap the map
*/
StringMap SymmetricEncryptor::initAlgorithmsMap()
{
    StringMap map, returned;
    OpenSSL_add_all_algorithms();

    // add all algorithms that could be used
    // names are listed in EVP_EncryptInit.pod
    map["BLOWFISH"]     = "bf";
    map["AES"]          = "aes";
    map["CAST5"]        = "cast5";
    map["IDEA"]         = "idea";
    map["3DES"]         = "des3";

    for (StringMap::iterator it = map.begin(); it != map.end(); ++it)
        if (EVP_get_cipherbyname(*it))
            returned[it.key()] = *it;

    return returned;
}


/*!
    \copydoc Encryptor::encrypt
*/
ByteVector SymmetricEncryptor::encrypt(const ByteVector& vector)
{
    return crypt(vector, ENCRYPT);
}


/*!
    \copydoc Encryptor::decrypt
*/
ByteVector SymmetricEncryptor::decrypt(const ByteVector& vector)
{
    return crypt(vector, DECRYPT);
}


/*!
    \enum SymmetricEncryptor::OperationType

    The operation type that indicates whether it should be encrypted (\c ENCRYPT) or
    decrypted (\c DECRYPT).
*/

/*!
    Does the real encryption/decryption according to the operation type.
*/
ByteVector SymmetricEncryptor::crypt(const ByteVector& vector, OperationType operation) const
{
    unsigned char buf[BUFLEN];
    unsigned char ebuf[BUFLEN + 8];
    ByteVector output;
    EVP_CIPHER_CTX ectx;
    int ebuflen;
    ByteVector::ConstIterator beginOfVector = vector.begin();
    unsigned int sizeOfVector = vector.size();
    EVP_CipherInit(&ectx, m_cipher_algorithm, m_key, m_iv, operation);

    for (unsigned int i = 0; i < sizeOfVector; i += BUFLEN) {
        int readLen = (i + BUFLEN >= sizeOfVector)
            ? sizeOfVector - i
            : BUFLEN;
        qCopy(beginOfVector + i, beginOfVector + i + readLen, buf);
        EVP_CipherUpdate(&ectx, ebuf, &ebuflen, buf, readLen);

        int oldSize = output.size();
        output.resize(oldSize + ebuflen);
        qCopy( ebuf, ebuf + ebuflen, output.begin() + oldSize);
    }

    EVP_CipherFinal(&ectx, ebuf, &ebuflen);

    int oldSize = output.size();
    output.resize(oldSize + ebuflen);
    qCopy(ebuf, ebuf + ebuflen, output.begin() + oldSize);

    EVP_CIPHER_CTX_cleanup(&ectx);

    return output;
}


/*!
    Sets a new a new password.
    \param password the password
*/
void SymmetricEncryptor::setPassword(const QString& password)
{
    Q3CString pwUtf8 = password.utf8();
    EVP_BytesToKey(m_cipher_algorithm, HASH_ALGORITHM, 0,
        (unsigned char *)pwUtf8.operator const char*(), pwUtf8.length(), 1, m_key, m_iv);
}


/*!
    Returns the cipher algorithm of this encryptor.
    \return the algorithm as string as used by the constructor
*/
QString SymmetricEncryptor::getCurrentAlgorithm() const
{
    return m_currentAlgorithm;
}

// :maxLineLen=100:
