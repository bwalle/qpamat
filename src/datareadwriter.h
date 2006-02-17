/*
 * Id: $Id$
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
#ifndef DATAREADWRITER_H
#define DATAREADWRITER_H

#include <stdexcept>

#include <QObject>
#include <QString>
#include <QWidget>
#include <QDomDocument>

#include "global.h"
#include "security/encryptor.h"

class ReadWriteException : public std::runtime_error
{
    public:
        enum Severity
        {
            CRITICAL, WARNING
        };
        enum Category
        {
            CWrongPassword, CInvalidData, CIOError, CSmartcardError, CNoAlgorithm, 
            CConfigurationError, COtherError, CAbort
        };
        
    public:
        ReadWriteException(const QString& error, Category category = COtherError, 
            Severity severity = WARNING) : std::runtime_error(error.isNull() ? "" : error.latin1()), 
            m_message(error), m_severity(severity), m_category(category) 
            { PRINT_TRACE("EXception generated"); }

        ~ReadWriteException() throw () {};

        QString getMessage() const
            { return m_message; }
        
        Severity getSeverity() const
            { return m_severity; }

        Category getCategory() const
            { return m_category; }
        
        bool retryMakesSense() const
            { return m_category == CWrongPassword || m_category == CSmartcardError; }

    private:
        const QString   m_message;
        const Severity  m_severity;
        const Category  m_category;
};

// ------------------------------------------------------------------------------------------------


class DataReadWriter
{
    public:
        DataReadWriter(QWidget* parent); 
        
    public:
        void writeXML(const QDomDocument& document, const QString& password)
            throw (ReadWriteException);
        
        QDomDocument readXML(const QString& password)
            throw (ReadWriteException);
            
        QDomDocument createSkeletonDocument() throw ();
    
    private:
        void writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber, 
            const QString& password) throw (ReadWriteException);
        void crypt(QDomElement& n, StringEncryptor& enc, bool encrypt);
        
    private:
        QWidget* m_parent;
};

#endif // DATAREADWRITER_H
