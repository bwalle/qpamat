/*
 * Id: $Id: property.cpp,v 1.7 2003/12/21 20:30:59 bwalle Exp $
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
#include <qdom.h>
#include <qlistview.h>

#include "qpamat.h"
#include "security/configpasswordchecker.h"
#include "property.h"
#include "security/encodinghelper.h"
#include "security/stringencryptor.h"
#include "treeentry.h"

// -------------------------------------------------------------------------------------------------
Property::Property(const QString& key, const QString& value, Type type, bool encrypted, bool hidden)
// -------------------------------------------------------------------------------------------------
         : m_key(key), m_value(value), m_type(type), m_encrypted(encrypted), m_hidden(hidden),
           m_weak(false)
{
    
    setValue(value);
}


// -------------------------------------------------------------------------------------------------
QString Property::getKey() const
// -------------------------------------------------------------------------------------------------
{
    return m_key;
}


// -------------------------------------------------------------------------------------------------
void Property::setKey(const QString& key)
// -------------------------------------------------------------------------------------------------
{
    m_key = key;
    emit propertyChanged(this);
}


// -------------------------------------------------------------------------------------------------
QString Property::getValue() const
// -------------------------------------------------------------------------------------------------
{
    return m_value;
}


// -------------------------------------------------------------------------------------------------
QString Property::getVisibleValue() const
// -------------------------------------------------------------------------------------------------
{
    if (m_hidden)
    {
        QString s;
        s.fill('*', m_value.length());
        return s;
    }
    else
    {
        return m_value;
    }
}


// -------------------------------------------------------------------------------------------------
bool Property::isWeak() const
// -------------------------------------------------------------------------------------------------
{
    return m_weak;
}


// -------------------------------------------------------------------------------------------------
void Property::setValue(const QString& value) 
// -------------------------------------------------------------------------------------------------
{
    m_value = value;
    updateWeakInformation();
    emit propertyChanged(this);
}


// -------------------------------------------------------------------------------------------------
void Property::updateWeakInformation()
// -------------------------------------------------------------------------------------------------
{
    if (m_type == PASSWORD)
    {
        int length = qpamat->set().readNumEntry("Security/MinLength");
        QString ensured = qpamat->set().readEntry("Security/EnsuredCharacters");
        ConfigPasswordChecker checker(length, ensured, QString::null);
        try
        {
            m_weak = !checker.isPasswordOk(m_value);
        }
        catch (const PasswordCheckException& e)
        {
            qDebug("Checking password failed, value = %s", m_value.latin1());
        }
    }
}


// -------------------------------------------------------------------------------------------------
Property::Type Property::getType() const
// -------------------------------------------------------------------------------------------------
{
    return m_type;
}


// -------------------------------------------------------------------------------------------------
void Property::setType(Property::Type type)
// -------------------------------------------------------------------------------------------------
{
    m_type = type;
    emit propertyChanged(this);
}


// -------------------------------------------------------------------------------------------------
bool Property::isHidden() const
// -------------------------------------------------------------------------------------------------
{
    return m_hidden;
}


// -------------------------------------------------------------------------------------------------
void Property::setHidden(bool hidden)
// -------------------------------------------------------------------------------------------------
{
    m_hidden = hidden;
    emit propertyChanged(this);
}


// -------------------------------------------------------------------------------------------------
bool Property::isEncrypted()
// -------------------------------------------------------------------------------------------------
{
    return m_encrypted;
}


// -------------------------------------------------------------------------------------------------
void Property::setEncrypted(bool encrypted)
// -------------------------------------------------------------------------------------------------
{
    m_encrypted = encrypted;
    emit propertyChanged(this);
}


// -------------------------------------------------------------------------------------------------
QString Property::toRichTextForPrint() const
// -------------------------------------------------------------------------------------------------
{
    return QString("<tr><td width=\"25%\">%1</td><td width=\"75%\">%2</td></tr>")
        .arg(m_key, m_value);
}


// -------------------------------------------------------------------------------------------------
void Property::appendXML(QDomDocument& document, QDomNode& parent, StringEncryptor& enc) const
// -------------------------------------------------------------------------------------------------
{
    QDomElement property = document.createElement("property");
    QString value = m_value;
    
    if (m_encrypted)
    {
        value = enc.encryptStrToStr(value);
    }
    
    property.setAttribute("key", m_key);
    property.setAttribute("value", value);
    property.setAttribute("hidden", m_hidden);
    property.setAttribute("encrypted", m_encrypted);
    
    QString type;
    switch (m_type)
    {
        case MISC    :  type = "MISC"    ;      break;
        case PASSWORD:  type = "PASSWORD";      break;
        case USERNAME:  type = "USERNAME";      break;
        case URL     :  type = "URL"     ;      break;
    }
    property.setAttribute("type", type);
    
    parent.appendChild(property);
}


// -------------------------------------------------------------------------------------------------
void Property::appendFromXML(TreeEntry* parent, QDomElement& element, StringEncryptor& enc)
// -------------------------------------------------------------------------------------------------
{
#ifdef DEBUG 
    Q_ASSERT( element.tagName() == "property" );
#endif
    
    QString key = element.attribute("key");
    QString value = element.attribute("value");
    bool hidden = element.attribute("hidden") == "1";
    bool encrypted = element.attribute("encrypted") == "1";
    QString typeString = element.attribute("type");
    
    Property::Type type;
    if (typeString == "USERNAME")
    {
        type = USERNAME;
    }
    else if (typeString == "PASSWORD")
    {
        type = PASSWORD;
    }
    else if (typeString == "URL")
    {
        type = URL;
    }
    else
    {
        type = MISC;
    }
    
    if (encrypted)
    {
        value = enc.decryptStrFromStr(value);
    }
    
    parent->appendProperty(new Property(key, value, type, encrypted, hidden));
}

