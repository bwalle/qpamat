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
#include <QString>
#include <QStringList>

#include "global.h"
#include "collectencryptor.h"

/**
 * @class CollectEncryptor
 *
 * @brief This encryptor just collects the encrypted bytes in a ByteArray.
 *
 * It is used for writing encrypted passwords to a smartcard. There are to modes of an
 * encrytor encrypting and decrypting. The real encrypting and decrypting operations are
 * passed to the real encryptor which is passed to the constructor of this object at
 * initialization time.
 *
 * In encrypt mode the CollectEncryptor takes a string, encrypts it and appends the
 * encrypted value in the byte vector. At the beginning, the vector is empty. The return
 * value of the CollectEncryptor::encryptString() method is a string like following: \c
 * SMARTCARD:0:27.  The first number after the colon indicates the offset in which the
 * encrypted bytes are stored in the byte array. The first string gets the offset \c 0, of
 * course. The second number is the length, i.e. the number of bytes. This means that the
 * byte vector now has a length of \c 27 bytes.
 *
 * In decrypt mode the CollectEncryptor does the opposite. It gets a string of the form \c
 * SMARTCARD:o:l, takes the bytes according to this specification from the byte array
 * (which must be set previously!) and tries to decrypt the value.
 *
 * @ingroup security
 * @author Bernhard Walle
 */

/**
 * @brief Creates a new instance of a CollectEncryptor object.
 *
 * @param encryptor the real encrytor used for encrypting
 */
CollectEncryptor::CollectEncryptor(Encryptor& encryptor)
    : m_realEncryptor(encryptor)
{}


/**
 * @copydoc StringEncryptor::encryptStrToStr
 */
QString CollectEncryptor::encryptStrToStr(const QString& string)
{
    ByteVector vec = m_realEncryptor.encryptStrToBytes(string);

    int oldSize = m_bytes.size();
    m_bytes.resize(oldSize + vec.size());
    qCopy(vec.begin(), vec.end(), m_bytes.begin() + oldSize);
    return QString("SMARTCARD:%1:%2").arg(QString::number(oldSize), QString::number(vec.size()));
}


/**
 * @copydoc StringEncryptor::decryptStrFromStr
 *
 * @exception std::invalid_argument if the argument is not of the specified form
 * @exception std::range_error if the byte array does not contain the specified bytes
 */
QString CollectEncryptor::decryptStrFromStr(const QString& string)
        throw (std::invalid_argument, std::range_error)
{
    QStringList list = QStringList::split(":", string);
    if (list.size() != 3) {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: string does not contain "
            "3 fields separated with \":\"");
    }
    if (list[0] != "SMARTCARD") {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[0] != "
            "\"SMARTCARD\"");
    }

    bool ok;
    int offset = list[1].toUInt(&ok);
    if (!ok) {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[1] is not a "
            "positive integer");
    }

    int length = list[2].toUInt(&ok);
    if (!ok) {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[1] is not a "
            "positive integer");
    }

    if (m_bytes.size() < offset + length) {
        throw std::invalid_argument(QString("CollectEncryptor::decryptStrFromStr: m_bytes is too "
            "small for the requested bytes: offset = %1, length = %d\n").arg(offset, length)
            .latin1());
    }

    ByteVector vec(length);
    ByteVector::iterator beg = m_bytes.begin() + offset;
    qCopy(beg, beg + length, vec.begin());
    return m_realEncryptor.decryptStrFromBytes(vec);
}


/**
 * Sets the stored bytes.
 *
 * \param vector the bytes
 */
void CollectEncryptor::setBytes(const ByteVector& vector)
{
    m_bytes = vector;
}


/**
 * Gets the stored bytes.
 */
ByteVector CollectEncryptor::getBytes()
{
    return m_bytes;
}

// vim: set sw=4 ts=4 et:
