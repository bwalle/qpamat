/*
 * Id: $Id: property.h,v 1.8 2003/12/21 20:30:59 bwalle Exp $
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
#ifndef PROPERTY_H
#define PROPERTY_H

#include <qstring.h>
#include <qdom.h>
#include <qobject.h>

#include "security/stringencryptor.h"

class TreeEntry;

/*!
 * \brief Represents one property.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.8 $
 * \date $Date: 2003/12/21 20:30:59 $
 */
class Property : public QObject
{
    Q_OBJECT
    
    friend class Tree;
    
    public:
    
        /*!
         * Type of the property that can be stored. QPaMaT has special features for
         * special types.
         */
        enum Type { MISC, USERNAME, PASSWORD, URL };

        /*!
         * Creates a new Property.
         * \param key the key of the property
         * \param value the value of a property
         * \param type the type of the property
         * \param encrypted whether the propertyp should be stored encrypted in the XML file
         * \param hidden whether the propertyp should be displayed as password on the screen
         */
        Property(const QString& key = QString::null, const QString& value = QString::null, 
            Type type = MISC, bool encrypted = false, bool hidden = false);
        
        /*!
         * Returns the key of the property
         * \return the key
         */
        QString getKey() const;
        
        /*!
         * Sets the key of the property.
         * \param key the new key
         */
        void setKey(const QString& key);
        
        /*!
         * Returns the value of the property.
         * \return the value
         */
        QString getValue() const;
        
        /*!
         * Returns the value that is visible to the user.
         * \return the value string
         */
        QString getVisibleValue() const;
        
        /*!
         * This function only makes sense if the property represents a password. It checkis
         * if the password is weak according to current password settings.
         * \return \c true if the password is weak, \c false otherwise
         */
        bool isWeak() const;
        
        /*!
         * Sets the value of the property.
         * \param value the new value
         */
        void setValue(const QString& value);
        
        /*!
         * Returns the type of the value.
         * \return the new type
         */
        Type getType() const;
         
        /*!
         * Sets the type of the property.
         * \param type the type
         */
        void setType(Type type);
        
        /*!
         * Returns whether the property should be displayed hidden.
         * \return \c true if the code should be displayed hidden, \c false otherwise
         */
        bool isHidden() const;
        
        /*!
         * Sets whether the property is displayed hidden.
         * \param hidden \c true if it should be displayed hidden, \c false otherwise
         */
        void setHidden(bool hidden);
        
        /*!
         * Returns whether this string should be stored encrypted. It's not stored encrypted
         * in the memory, only after saving to XML.
         * \return \c true if the code should be stored encrypted, \c false otherwise
         */
        bool isEncrypted();
        
        /*!
         * Controls whether this string should be stored encrypted in XML.
         * \param encrypted \c true if the code should be stored encrypted, \c false otherwise
         */
        void setEncrypted(bool encrypted);
        
        /*!
         * Converts the property to rich text which is used to printing. One property represents
         * a row in a table, so it begins with an \c \<tr\> tag end ends with an \c \</tr\> tag.
         * \return the rich text
         */
        QString toRichTextForPrint() const;
        
        /*!
         * Appends the property as \c property tag in the XML structure.
         * \param document the document needed to create new elements
         * \param parent the parent to which the new created element should be attached
         * \param enc the encryptor to use for encrypting passwords
         */
        void appendXML(QDomDocument& document, QDomNode& parent, StringEncryptor& enc) const;
        
        /*!
         * Creates a Propery element from a XML \c property tag.
         * \param parent the parent
         * \param elem the \c property tag
         * \param enc the encryptor to use for decrypting passwords
         */
        static void appendFromXML(TreeEntry* parent, QDomElement& elem, StringEncryptor& enc);
        
    signals:
        /*!
         * This signal is emited if some property is changed.
         * \param current the this pointer
         */
        void propertyChanged(Property* current);
        
    private:
        void updateWeakInformation();
        
    private:
        QString         m_key;
        QString         m_value;
        Type            m_type;
        bool            m_encrypted;
        bool            m_hidden;
        bool            m_weak;
};

#endif // PROPERTY_H
