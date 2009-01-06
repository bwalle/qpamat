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
#ifndef SYMMETRICENCRYPTOR_H
#define SYMMETRICENCRYPTOR_H

#include <QString>
#include <QStringList>

#include <openssl/evp.h>

#include "global.h"
#include "abstractencryptor.h"

class SymmetricEncryptor : public AbstractEncryptor
{
    public:
        SymmetricEncryptor(const QString& algorithm, const QString& password)
            throw (NoSuchAlgorithmException);

        virtual QString getCurrentAlgorithm() const;
        static QStringList getAlgorithms();

        ByteVector encrypt(const ByteVector& vector);
        ByteVector decrypt(const ByteVector& vector);

        virtual void setPassword(const QString& password);
        static QString getSuggestedAlgorithm();

    protected:
        enum OperationType {
            DECRYPT, ENCRYPT
        };

    protected:
        virtual ByteVector crypt(const ByteVector& vector, OperationType operation) const;

    private:
        const EVP_CIPHER*     m_cipher_algorithm;
        mutable unsigned char m_key[EVP_MAX_KEY_LENGTH];
        mutable unsigned char m_iv[EVP_MAX_IV_LENGTH];
        QString               m_currentAlgorithm;

    private:
        static StringMap initAlgorithmsMap();
        static StringMap m_algorithms;
};

#endif // SYMMETRICENCRYPTOR_H

// :maxLineLen=100:
