/*
 * Id: $Id: collectencryptor.cpp,v 1.2 2003/12/10 21:47:47 bwalle Exp $
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
#include <qstringlist.h>

#include "nosuchalgorithmexception.h"
#include "../global.h"
#include "collectencryptor.h"


// -------------------------------------------------------------------------------------------------
CollectEncryptor::CollectEncryptor(Encryptor& encryptor)
// -------------------------------------------------------------------------------------------------
            : m_realEncryptor(encryptor)
{}


// -------------------------------------------------------------------------------------------------
QString CollectEncryptor::encryptStrToStr(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    ByteVector vec = m_realEncryptor.encryptStrToBytes(string);
    
    int oldSize = m_bytes.size();
    m_bytes.resize(oldSize + vec.size());
    qCopy(vec.begin(), vec.end(), m_bytes.begin() + oldSize);
    return QString("SMARTCARD:%1:%2").arg(QString::number(oldSize), QString::number(vec.size()));
}


// -------------------------------------------------------------------------------------------------
QString CollectEncryptor::decryptStrFromStr(const QString& string)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument, std::range_error)
{
    QStringList list = QStringList::split(":", string);
    if (list.size() != 3)
    {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: string does not contain "
            "3 fields separated with \":\"");
    }
    if (list[0] != "SMARTCARD")
    {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[0] != "
            "\"SMARTCARD\"");
    }
    bool ok;
    uint offset = list[1].toUInt(&ok);
    if (!ok)
    {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[1] is not a "
            "positive integer");
    }
    uint length = list[2].toUInt(&ok);
    if (!ok)
    {
        throw std::invalid_argument("CollectEncryptor::decryptStrFromStr: list[1] is not a "
            "positive integer");
    }
    
    if (m_bytes.size() < offset + length)
    {
        throw std::invalid_argument(QString("CollectEncryptor::decryptStrFromStr: m_bytes is too "
            "small for the requested bytes: offset = %1, length = %d\n").arg(offset, length)
            .latin1());
    }
    ByteVector vec(length);
    ByteVector::iterator beg = m_bytes.begin() + offset;
    qCopy(beg, beg + length, vec.begin()); 
    return m_realEncryptor.decryptStrFromBytes(vec);
}


// -------------------------------------------------------------------------------------------------
void CollectEncryptor::setBytes(const ByteVector& vector)
// -------------------------------------------------------------------------------------------------
{
    m_bytes = vector;
}


// -------------------------------------------------------------------------------------------------
ByteVector CollectEncryptor::getBytes()
// -------------------------------------------------------------------------------------------------
{
    return m_bytes;
}

