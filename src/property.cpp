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
#include <QDomDocument>
#include <Q3ListView>
#include <QMessageBox>
#include <QTextStream>

#include "qpamatwindow.h"
#include "qpamat.h"
#include "util/securestring.h"
#include "security/hybridpasswordchecker.h"
#include "property.h"
#include "security/encodinghelper.h"
#include "treeentry.h"

/**
 * @class PropertyValue
 *
 * @brief Represents the value of a property.
 *
 * This is some kind of union class for a QString and a SecureString.
 *
 * @ingroup gui
 * @author Bernhard Walle
 */


/**
 * @brief Constructor
 *
 * Creates a new instance of a PropertyValue.
 *
 * @param[in] string the value as string
 * @param[in] storeSecure @c true if the string should be stored as SecureString,
 *                        @c false otherwise
 */
PropertyValue::PropertyValue(const QString &string, bool storeSecure)
    : m_string(QString::null)
    , m_secureString()
    , m_isSecureString(false)
{
    set(string, storeSecure);
}


/**
 * @brief Sets a value to a PropertyValue
 *
 * Sets a value.
 *
 * @param[in] string the value as string
 * @param[in] storeSecure @c true if the string should be stored as SecureString,
 *                        @c false otherwise
 */
void PropertyValue::set(const QString &string, bool storeSecure)
{
    if (storeSecure) {
        m_secureString = SecureString(string);
        m_isSecureString = true;
    } else {
        m_string = string;
        m_isSecureString = false;
    }
}


/**
 * @brief Returns the value
 *
 * Returns the value as QString.
 *
 * @return the value
 */
QString PropertyValue::get() const
{
    if (m_isSecureString) {
        return m_secureString.qString();
    } else {
        return m_string;
    }
}


/**
 * @brief Returns the visible value
 *
 * The same as get(), but in case the string is stored secure, all characters are replaced
 * with stars <tt>(*)</tt>.
 *
 * @return the visible value as string
 */
QString PropertyValue::getVisible() const
{
    if (m_isSecureString) {
        QString s;
        s.fill('*', m_secureString.length());
        return s;
    } else {
        return m_string;
    }
}


/**
 * @class Property
 *
 * @brief Represents one property.
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @enum Property::Type
 *
 * @brief Type of the property that can be stored.
 *
 * QPaMaT has special features for special types.
 */

/**
 * @enum Property::PasswordStrength
 *
 * @brief Enumeration type for the passwords strength.
 *
 * The meaning of these types depends on the settings. The meaning depends on
 * the user setting. \c PUndefined is a escape value.
 */

/**
 * @fn Property::propertyChanged(Property*)
 *
 * @brief This signal is emited if some property is changed.
 *
 * @param current the this pointer
 */

/**
 * @brief Creates a new Property.
 *
 * @param key the key of the property
 * @param value the value of a property
 * @param type the type of the property
 * @param encrypted whether the propertyp should be stored encrypted in the XML file
 * @param hidden whether the propertyp should be displayed as password on the screen
 */
Property::Property(const QString& key, const QString& value, Type type, bool encrypted, bool hidden)
    : m_key(key)
    , m_value(value)
    , m_type(type)
    , m_encrypted(encrypted)
    , m_hidden(hidden)
    , m_passwordStrength(PUndefined)
    , m_daysToCrack(-1.0)
{
    setValue(value);
}


/**
 * @brief Returns the key of the property
 *
 * @return the key
 */
QString Property::getKey() const
{
    return m_key;
}


/**
 * @brief Sets the key of the property.
 *
 * @param key the new key
 */
void Property::setKey(const QString& key)
{
    m_key = key;
    emit propertyChanged(this);
}


/**
 * @brief Returns the value of the property.
 *
 * @return the value
 */
QString Property::getValue() const
{
    return m_value.get();
}


/**
 * @brief Returns the value that is visible to the user.
 *
 * @return the value string
 */
QString Property::getVisibleValue() const
{
    return m_value.getVisible();
}


/**
 * @brief Sets the value of the property.
 *
 * @param value the new value
 */
void Property::setValue(const QString& value)
{
    m_value.set(value, m_hidden);
    emit propertyChanged(this);
}


/**
 * @brief This function only makes sense if the property represents a password.
 *
 * It returns the password strength. The value is cached. Recomputing takes
 * place the first time this function is called and any thimes the
 * updatePasswordStrength() function is called. There's no automatic
 * recomputation because of performance reasons.
 *
 * @return the password strength which is \c PUndefined if it is no password
 * @exception PasswordCheckException if the strength is updated and a PasswordCheckException
 *            is thrown
 */
Property::PasswordStrength Property::getPasswordStrength() throw (PasswordCheckException)
{
    if (m_passwordStrength == PUndefined)
        updatePasswordStrength();
    return m_passwordStrength;
}


/**
 * @brief This function ony makes sense if the property represents a password.
 *
 * It returns the days a cracker needs to try to crack the password according
 * to the HybridPasswordChecker and the current settings. Recomputing takes
 * place if the updateWeakInformation() member function is called.
 *
 * @return the days or a negative value if this is no password
 */
double Property::daysToCrack() const
{
    return m_daysToCrack;
}


/**
 * @brief Updates the password strength information.
 *
 * Because updating this information may be expensive, you sometimes manually
 * must call this function.
 *
 * @exception if the password checker threw a PasswordCheckException
 */
void Property::updatePasswordStrength() throw (PasswordCheckException)
{
    if (m_type == PASSWORD) {
        QpamatWindow *win = Qpamat::instance()->getWindow();
        QString ensured = win->set().readEntry("Security/EnsuredCharacters");
        double days = -1.0;
        HybridPasswordChecker checker(win->set().readEntry("Security/DictionaryFile"));
        days = checker.passwordQuality(m_value.get()); // XXX
        m_daysToCrack = days;
        double weakLimit = win->set().readDoubleEntry("Security/WeakPasswordLimit");
        double strongLimit = win->set().readDoubleEntry("Security/StrongPasswordLimit");
        if (m_daysToCrack < weakLimit)
            m_passwordStrength = PWeak;
        else if (m_daysToCrack >= weakLimit && m_daysToCrack < strongLimit)
            m_passwordStrength = PAcceptable;
        else
            m_passwordStrength = PStrong;
    }
}


/**
 * @brief Returns the type of the value.
 *
 * @return the new type
 */
Property::Type Property::getType() const
{
    return m_type;
}


/**
 * @brief Sets the type of the property.
 *
 * @param type the type
 */
void Property::setType(Property::Type type)
{
    m_type = type;
    emit propertyChanged(this);
}


/**
 * @brief Returns whether the property should be displayed hidden.
 *
 * @return \c true if the code should be displayed hidden, \c false otherwise
 */
bool Property::isHidden() const
{
    return m_hidden;
}


/**
 * @brief Sets whether the property is displayed hidden.
 *
 * @param hidden \c true if it should be displayed hidden, \c false otherwise
 */
void Property::setHidden(bool hidden)
{
    m_hidden = hidden;
    emit propertyChanged(this);
}


/**
 * @brief Returns whether this string should be stored encrypted.
 *
 * It's not stored encrypted in the memory, only after saving to XML.
 *
 * @return \c true if the code should be stored encrypted, \c false otherwise
 */
bool Property::isEncrypted()
{
    return m_encrypted;
}


/**
 * @brief Controls whether this string should be stored encrypted in XML.
 *
 * @param encrypted \c true if the code should be stored encrypted, \c false otherwise
 */
void Property::setEncrypted(bool encrypted)
{
    m_encrypted = encrypted;
    emit propertyChanged(this);
}


/**
 * @brief Converts the property to rich text which is used to printing.
 *
 * One property represents a row in a table, so it begins with an \c \<tr\>
 * tag end ends with an \c \</tr\> tag.
 *
 * @return the rich text
 */
QString Property::toRichTextForPrint() const
{
    return QString("<tr><td width=\"25%\">%1</td><td width=\"75%\">%2</td></tr>")
        .arg(m_key, getValue());
}


/**
 * @brief Appends the tree entry as text representation to the given stream.
 *
 * The text is formatted for export.
 *
 * @param stream the stream where the text is appended
 */
void Property::appendTextForExport(QTextStream& stream)
{
    stream << qSetFieldWidth(20) << m_key + ": " << qSetFieldWidth(0) << getValue() << "\n";
}


/**
 * @brief Appends the property as \c property tag in the XML structure.
 *
 * No encryption is done, the XML structure must be encrypted afterwards.
 *
 * @param document the document needed to create new elements
 * @param parent the parent to which the new created element should be attached
 */
void Property::appendXML(QDomDocument& document, QDomNode& parent) const
{
    QDomElement property = document.createElement("property");
    QString value = getValue();

    property.setAttribute("key", m_key);
    property.setAttribute("value", value);
    property.setAttribute("hidden", m_hidden);
    property.setAttribute("encrypted", m_encrypted);

    QString type;
    switch (m_type) {
        case MISC    :
            type = "MISC";
            break;

        case PASSWORD:
            type = "PASSWORD";
            break;

        case USERNAME:
            type = "USERNAME";
            break;

        case URL:
            type = "URL";
            break;
    }
    property.setAttribute("type", type);

    parent.appendChild(property);
}


/**
 * @brief Creates a Propery element from a XML \c property tag.
 *
 * @param parent the parent
 * @param element the \c property tag
 */
void Property::appendFromXML(TreeEntry* parent, QDomElement& element)
{
    Q_ASSERT( element.tagName() == "property" );

    QString key = element.attribute("key");
    QString value = element.attribute("value");
    bool hidden = element.attribute("hidden") == "1";
    bool encrypted = element.attribute("encrypted") == "1";
    QString typeString = element.attribute("type");

    Property::Type type;
    if (typeString == "USERNAME")
        type = USERNAME;
    else if (typeString == "PASSWORD")
        type = PASSWORD;
    else if (typeString == "URL")
        type = URL;
    else
        type = MISC;

    parent->appendProperty(new Property(key, value, type, encrypted, hidden));
}

// vim: set sw=4 ts=4 et:
