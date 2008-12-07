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
#include <QSettings>
#include <QDir>
#include <QApplication>

#include "security/symmetricencryptor.h"
#include "security/passwordgeneratorfactory.h"
#include "settings.h"
#include "security/encryptor.h"


/*!
    \class Settings
    
    \brief Singleton for storing settings in registry (MS Windows) or ini-file (Unix).
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.17 $
    \date $Date$
*/

/*!
    Creates a new instance of the settings object. The default values are initialized.
*/
Settings::Settings()
{
    m_qSettings.setPath( "qpamat", "qpamat", QSettings::User );
    
#define DEF_STRING(a, b) ( m_stringMap.insert( (a), (b) ) )
#define DEF_INTEGE(a, b) ( m_intMap.insert( (a), (b) ) )
#define DEF_DOUBLE(a, b) ( m_doubleMap.insert( (a), (b) ) )
#define DEF_BOOLEA(a, b) ( m_boolMap.insert( (a), (b) ) )
    
    DEF_BOOLEA("Main Window/maximized",          false);
    DEF_STRING("Main Window/SearchHistory",      "");
    DEF_STRING("Main Window/Layout",             "");
    DEF_STRING("General/Datafile",               QDir::homeDirPath() + "/.qpamat");
    DEF_BOOLEA("General/AutoLogin",              true);
    DEF_STRING("AutoText/Misc",                  "");
    DEF_STRING("AutoText/Username",              "Username");
    DEF_STRING("AutoText/Password",              "Password");
    DEF_STRING("AutoText/URL",                   "URL");
    DEF_STRING("Security/CipherAlgorithm",       SymmetricEncryptor::getSuggestedAlgorithm());
    DEF_INTEGE("Security/Length",                8);
    DEF_STRING("Security/AllowedCharacters",     "a-zA-Z0-9@$#");
    DEF_DOUBLE("Security/WeakPasswordLimit",     3.0);
    DEF_DOUBLE("Security/StrongPasswordLimit",   15.0);
    DEF_STRING("Security/DictionaryFile",        QDir(qApp->applicationDirPath() + "/../share/"
                                                 "qpamat/dicts").canonicalPath() + "/default.txt");
    DEF_STRING("Security/PasswordGenerator",     PasswordGeneratorFactory::DEFAULT_GENERATOR_STRING);
    DEF_STRING("Security/PasswordGenAdditional", "");
    DEF_INTEGE("Security/AutoLogout",            0);
    DEF_BOOLEA("Smartcard/Library",              false);
    DEF_INTEGE("Smartcard/Port",                 1);
    DEF_STRING("Smartcard/Library",              "");
    DEF_BOOLEA("Smartcard/HasWriteProtection",   false);
    DEF_BOOLEA("Smartcard/UseCard",              false);
    DEF_BOOLEA("Password/NoGrabbing",            false);
#ifdef Q_WS_WIN                                       
    DEF_STRING("Presentation/NormalFont",        "Times New Roman,10");
    DEF_STRING("Presentation/FooterFont",        "Arial,9");
#else                                                 
    DEF_STRING("Presentation/NormalFont",        "Times,10");
    DEF_STRING("Presentation/FooterFont",        "Helvetica,9");
#endif
    DEF_BOOLEA("Presentation/HideRandomPass",    false);
    DEF_BOOLEA("Presentation/SystemTrayIcon",    false);
    DEF_BOOLEA("Presentation/StartHidden",       false);
    
    
#undef DEF_STRING
#undef DEF_INTEGE
#undef DEF_DOUBLE
#undef DEF_BOOLEA
}


/*!
    \fn Settings::~Settings()
    
    Destructor
*/

/*!
    Writes an entry into the settings.
    \param key the key
    \param value the value
    \return if the entry was written
*/
bool Settings::writeEntry(const QString & key, const QString & value)
{
    return m_qSettings.writeEntry(key, value);
}


/*!
    Writes an entry into the settings.
    \param key the key
    \param value the value
    \return if the entry was written
*/
bool Settings::writeEntry(const QString & key, double value)
{
    return m_qSettings.writeEntry(key, value);
}


/*!
    Writes an entry into the settings.
    \param key the key
    \param value the value
    \return if the entry was written
*/
bool Settings::writeEntry(const QString & key, int value)
{
    return m_qSettings.writeEntry(key, value);
}


/*!
    Writes an entry into the settings.
    \param key the key
    \param value the value
    \return if the entry was written
*/
bool Settings::writeEntry(const QString & key, bool value)
{
    return m_qSettings.writeEntry(key, value);
}


/*!
    Stores a byte array.
    
    \param key the key
    \param bytes the value
    \return if the entry was written
*/
bool Settings::writeEntry(const QString& key, const QByteArray& bytes)
{
    return m_qSettings.writeEntry(key, QString::fromUtf8(bytes.toBase64()));
    
}


/*!
    Reads the entry. If the entry does not exist there's a automatic default value that
    is returned.
    \return the entry
*/
QString Settings::readEntry(const QString & key, const QString& def)
{
    bool read = false;
    QString string = m_qSettings.readEntry(key, def, &read);
    if (!read && m_stringMap.contains(key))
    {
        return m_stringMap[key];
    }
#ifdef DEBUG
    else if (!read && def.isNull())
    {
        PRINT_DBG("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return string;
}


/*!
    Reads a number entry. If the entry does not exist there's a automatic default value that
    is returned.
    \return the entry
*/
int Settings::readNumEntry (const QString & key, int def)
{
    bool read = m_qSettings.contains(key);
    int number = m_qSettings.value(key, def).toInt();
    if (!read && m_intMap.contains(key))
    {
        return m_intMap[key];
    }
#ifdef DEBUG
    else if (!read && def == 0)
    {
        PRINT_TRACE("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return number;
}


/*!
    Reads a double entry. If the entry does not exist there's a automatic default value that
    is returned.
    \return the entry
*/
double Settings::readDoubleEntry(const QString & key, double def) const
{
    bool read = m_qSettings.contains(key);
    double number = m_qSettings.value(key, def).toDouble();
    if (!read && m_doubleMap.contains(key))
    {
        return m_doubleMap[key];
    }
#ifdef DEBUG
    else if (!read && def == 0.0)
    {
        PRINT_TRACE("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return number;
}


/*!
    Reads a boolean entry. If the entry does not exist there's a automatic default value that
    is returned.
    \return the entry
*/
bool Settings::readBoolEntry(const QString & key, bool def) const
{
    bool read = m_qSettings.contains(key);
    bool res = m_qSettings.value(key, def).toBool();
    if (!read && m_boolMap.contains(key))
    {
        return m_boolMap[key];
    }
#ifdef DEBUG
    else if (!read && !def)
    {
        PRINT_TRACE("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return res;
}


/*!
    Writes a QByteArray entry. If the entry does not exist, the default value is returned.
    There's no automatic default value for QByteArrays.
    
    \param key the key to look for
    \param def the default value if the key does not exist
    \return the entry
*/
QByteArray Settings::readByteArrayEntry(const QString& key, const QByteArray& def)
{
    if (m_qSettings.contains(key))
    {
        return QByteArray::fromBase64(m_qSettings.value(key).toString().toUtf8());
    }
    else
    {
        return def;
    }
}


// :maxLineLen=100:
