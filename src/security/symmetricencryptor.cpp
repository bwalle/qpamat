/*
 * Id: $Id: symmetricencryptor.cpp,v 1.2 2003/12/10 21:48:13 bwalle Exp $
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
#include <iostream>
#include <qstring.h>
#include <qstringlist.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>

#include "symmetricencryptor.h"
#include "constants.h"
#include "nosuchalgorithmexception.h"
#include "encodinghelper.h"

#ifndef BUFLEN
#define BUFLEN 512
#endif

// -------------------------------------------------------------------------------------------------
StringMap SymmetricEncryptor::m_algorithms = initAlgorithmsMap();
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
SymmetricEncryptor::SymmetricEncryptor(const QString& algorithm, const QString& password)
            throw (NoSuchAlgorithmException)
// -------------------------------------------------------------------------------------------------
{
    // set the right cipher algorithm
    if (m_algorithms.contains(algorithm.upper()))
    {
        m_cipher_algorithm = EVP_get_cipherbyname(m_algorithms[algorithm.upper()]);
    }
    else
    {
        throw NoSuchAlgorithmException(("Algorithm "+algorithm+" not supported").latin1());
    }
    
    // set the password
    setPassword(password);
    m_currentAlgorithm = algorithm;
}


// -------------------------------------------------------------------------------------------------
QStringList SymmetricEncryptor::getAlgorithms()
// -------------------------------------------------------------------------------------------------
{
    QStringList algorithms;
    
    for (StringMap::iterator it = m_algorithms.begin(); it != m_algorithms.end(); ++it)
    {
        algorithms += it.key();
    }
    
    
    return algorithms;
}


// -------------------------------------------------------------------------------------------------
QString SymmetricEncryptor::getSuggestedAlgorithm()
// -------------------------------------------------------------------------------------------------
{
    StringVector vec;
    vec.push_back("BLOWFISH");
    vec.push_back("AES");
    vec.push_back("CAST5");
    
    for (StringVector::Iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (m_algorithms.contains(*it))
        {
            return *it;
        }
    }
    return QString();
}


// -------------------------------------------------------------------------------------------------
StringMap SymmetricEncryptor::initAlgorithmsMap()
// -------------------------------------------------------------------------------------------------

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
    {
        if (EVP_get_cipherbyname(*it))
        {
            returned[it.key()] = *it;
        }
    }
    return returned;
}

// -------------------------------------------------------------------------------------------------
ByteVector SymmetricEncryptor::encrypt(const ByteVector& vector)
// -------------------------------------------------------------------------------------------------
{
    return crypt(vector, ENCRYPT);
}


// -------------------------------------------------------------------------------------------------
ByteVector SymmetricEncryptor::decrypt(const ByteVector& vector)
// -------------------------------------------------------------------------------------------------
{
    return crypt(vector, DECRYPT);
}


// -------------------------------------------------------------------------------------------------
ByteVector SymmetricEncryptor::crypt(const ByteVector& vector, OperationType operation) const
// -------------------------------------------------------------------------------------------------
{
    byte buf[BUFLEN];
	byte ebuf[BUFLEN + 8];
    ByteVector output;
    EVP_CIPHER_CTX ectx;
    int ebuflen;
    ByteVector::ConstIterator beginOfVector = vector.begin();
    uint sizeOfVector = vector.size();
    EVP_CipherInit(&ectx, m_cipher_algorithm, m_key, m_iv, operation);
    
    for (uint i = 0; i < sizeOfVector; i += BUFLEN)
    {
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
    qCopy( ebuf, ebuf + ebuflen, output.begin() + oldSize);
    
    EVP_CIPHER_CTX_cleanup(&ectx);
    
    return output;
}


// -------------------------------------------------------------------------------------------------
void SymmetricEncryptor::setPassword(const QString& password)
// -------------------------------------------------------------------------------------------------
{
    QCString pwUtf8 = password.utf8();
    EVP_BytesToKey(m_cipher_algorithm, HASH_ALGORITHM, 0, 
        (byte*)pwUtf8.operator const char*(), pwUtf8.length(), 1, m_key, m_iv);
}


// -------------------------------------------------------------------------------------------------
QString SymmetricEncryptor::getCurrentAlgorithm() const
// -------------------------------------------------------------------------------------------------
{
    return m_currentAlgorithm;
}


