/*
 * Id: $Id: property.cpp,v 1.1 2003/10/05 16:08:21 bwalle Exp $
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

#include "property.h"
#include "cipher/encodinghelper.h"
#include "cipher/encryptor.h"
#include "treeentry.h"

// -------------------------------------------------------------------------------------------------
Property::Property(const QString& key, const QString& value, Type type, bool encrypted, bool hidden)
// -------------------------------------------------------------------------------------------------
        : m_key(key), m_value(value), m_type(type), m_encrypted(encrypted), m_hidden(hidden)
{ }


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
}


// -------------------------------------------------------------------------------------------------
QString Property::getValue() const
// -------------------------------------------------------------------------------------------------
{
    return m_value;
}


// -------------------------------------------------------------------------------------------------
void Property::setValue(const QString& value) 
// -------------------------------------------------------------------------------------------------
{
    m_value = value;
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
}


// -------------------------------------------------------------------------------------------------
void Property::setHidden(bool hidden)
// -------------------------------------------------------------------------------------------------
{
    m_hidden = hidden;
}


// -------------------------------------------------------------------------------------------------
bool Property::isHidden() const
// -------------------------------------------------------------------------------------------------
{
    return m_hidden;
}

// -------------------------------------------------------------------------------------------------
void Property::appendXML(QDomDocument& document, QDomNode& parent, const Encryptor& enc) const
// -------------------------------------------------------------------------------------------------
{
    QDomElement property = document.createElement("property");
    QString value = m_value;
    
    if (m_encrypted)
    {
        value = EncodingHelper::toBase64(enc.encryptString(value));
    }
    
    property.setAttribute("key", m_key);
    property.setAttribute("value", value);
    property.setAttribute("hidden", m_hidden);
    property.setAttribute("encrypted", m_encrypted);
    
    QString type;
    switch (m_type)
    {
        case NORMAL  :  type = "NORMAL"  ;      break;
        case PASSWORD:  type = "PASSWORD";      break;
        case USERNAME:  type = "USERNAME";      break;
    }
    property.setAttribute("type", type);
    
    parent.appendChild(property);
}


// -------------------------------------------------------------------------------------------------
void Property::appendFromXML(TreeEntry* parent, QDomElement& element, const Encryptor& enc)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_STATE 
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
    else
    {
        type = NORMAL;
    }
    
    if (encrypted)
    {
        value = enc.decryptString(EncodingHelper::fromBase64(value));
    }
    
    parent->appendProperty(new Property(key, value, type, encrypted, hidden));
}

