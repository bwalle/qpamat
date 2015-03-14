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
#ifndef PROPERTY_H
#define PROPERTY_H

#include <QString>
#include <QDomDocument>
#include <QObject>
#include <QTextStream>

#include "util/securestring.h"
#include "security/passwordchecker.h"

class TreeEntry;

class PropertyValue
{
    public:
        PropertyValue(const QString &string, bool storeSecure=false);

    public:
        void set(const QString &string, bool storeSecure=false);
        QString get() const;
        QString getVisible() const;

    private:
        QString         m_string;
        SecureString    m_secureString;
        bool            m_isSecureString;
};

class Property : public QObject
{
    Q_OBJECT

    friend class Tree;

    public:
        enum Type {
            MISC,
            USERNAME,
            PASSWORD,
            URL
        };

        enum PasswordStrength {
            PWeak,
            PAcceptable,
            PStrong,
            PUndefined
        };

    public:
        Property(const QString& key = QString::null, const QString& value = QString::null,
            Type type = MISC, bool encrypted = false, bool hidden = false);

        QString getKey() const;
        void setKey(const QString& key);

        QString getValue() const;
        void setValue(const QString& value);
        QString getVisibleValue() const;

        PasswordStrength getPasswordStrength();
        void updatePasswordStrength();
        double daysToCrack() const;

        Type getType() const;
        void setType(Property::Type type);

        bool isHidden() const;
        void setHidden(bool hidden);

        bool isEncrypted();
        void setEncrypted(bool encrypted);

        QString toRichTextForPrint() const;
        void appendTextForExport(QTextStream& stream);


        void appendXML(QDomDocument& document, QDomNode& parent) const;

    public:
        static void appendFromXML(TreeEntry* parent, QDomElement& elem);

    signals:
        void propertyChanged(Property* current);

    private:
        QString          m_key;
        PropertyValue    m_value;
        Type             m_type;
        bool             m_encrypted;
        bool             m_hidden;
        PasswordStrength m_passwordStrength;
        double           m_daysToCrack;
};

#endif // PROPERTY_H

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
