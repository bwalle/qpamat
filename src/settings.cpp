/*
 * Id: $Id: settings.cpp,v 1.8 2003/12/21 20:30:26 bwalle Exp $
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
#include <qsettings.h>
#include <qdir.h>

#include "security/symmetricencryptor.h"
#include "security/passwordgeneratorfactory.h"
#include "settings.h"
#include "security/encryptor.h"

// -------------------------------------------------------------------------------------------------
Settings::Settings()
// -------------------------------------------------------------------------------------------------
{
    m_qSettings.setPath( "bwalle.de", "qpamat", QSettings::User );
    m_qSettings.beginGroup("/qpamat");
    
#define DEF_STRING(a, b) ( m_stringMap.insert( (a), (b) ) )
#define DEF_INTEGE(a, b) ( m_intMap.insert( (a), (b) ) )
#define DEF_DOUBLE(a, b) ( m_doubleMap.insert( (a), (b) ) )
#define DEF_BOOLEA(a, b) ( m_boolMap.insert( (a), (b) ) )
    
    DEF_BOOLEA("Main Window/maximized",          false);
    DEF_BOOLEA("General/ShowWeakPasswords",      false);
    DEF_STRING("Main Window/SearchHistory",      "");
    DEF_STRING("Main Window/Layout",             "");
    DEF_STRING("General/Webbrowser",             "mozilla");
    DEF_STRING("General/Datafile",               QDir::home().absPath() + "/.qpamat");
    DEF_BOOLEA("General/AutoLogin",              true);
    DEF_STRING("AutoText/Misc",                  "");
    DEF_STRING("AutoText/Username",              "Username");
    DEF_STRING("AutoText/Password",              "Password");
    DEF_STRING("AutoText/URL",                   "URL");
    DEF_STRING("Security/CipherAlgorithm",       SymmetricEncryptor::getSuggestedAlgorithm());
    DEF_STRING("Security/EnsuredCharacters",     "ULDs");
    DEF_INTEGE("Security/Length",                8);
    DEF_INTEGE("Security/MinLength",             6);
    DEF_STRING("Security/AllowedCharacters",     "a-zA-Z0-9@$#");
    DEF_STRING("Security/PasswordGenerator",     PasswordGeneratorFactory::DEFAULT_GENERATOR_STRING);
    DEF_STRING("Security/PasswordGenAdditional", "");
#ifdef Q_WS_WIN                                       
    DEF_STRING("Presentation/NormalFont",        "Times New Roman,10");
    DEF_STRING("Presentation/FooterFont",        "Arial,9");
#else                                                 
    DEF_STRING("Presentation/NormalFont",        "Times,10");
    DEF_STRING("Presentation/FooterFont",        "Helvetica,9");
#endif
    DEF_BOOLEA("Presentation/HideRandomPass",    false);
    
    
#undef DEF_STRING
#undef DEF_INTEGE
#undef DEF_DOUBLE
#undef DEF_BOOLEA
}


// -------------------------------------------------------------------------------------------------
bool Settings::writeEntry(const QString & key, const QString & value)
// -------------------------------------------------------------------------------------------------
{
    return m_qSettings.writeEntry(key, value);
}


// -------------------------------------------------------------------------------------------------
bool Settings::writeEntry(const QString & key, double value)
// -------------------------------------------------------------------------------------------------
{
    return m_qSettings.writeEntry(key, value);
}


// -------------------------------------------------------------------------------------------------
bool Settings::writeEntry(const QString & key, int value)
// -------------------------------------------------------------------------------------------------
{
    return m_qSettings.writeEntry(key, value);
}


// -------------------------------------------------------------------------------------------------
bool Settings::writeEntry(const QString & key, bool value)
// -------------------------------------------------------------------------------------------------
{
    return m_qSettings.writeEntry(key, value);
}


// -------------------------------------------------------------------------------------------------
QString Settings::readEntry(const QString & key, const QString& def)
// -------------------------------------------------------------------------------------------------
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
        qDebug("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return string;
}


// -------------------------------------------------------------------------------------------------
int Settings::readNumEntry (const QString & key, int def)
// -------------------------------------------------------------------------------------------------
{
    bool read = false;
    int number = m_qSettings.readNumEntry(key, def, &read);
    if (!read && m_intMap.contains(key))
    {
        return m_intMap[key];
    }
#ifdef DEBUG
    else if (!read && def == 0)
    {
        qDebug("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return number;
}


// -------------------------------------------------------------------------------------------------
double Settings::readDoubleEntry(const QString & key, double def) const
// -------------------------------------------------------------------------------------------------
{
    bool read = false;
    double number = m_qSettings.readDoubleEntry(key, def, &read);
    if (!read && m_doubleMap.contains(key))
    {
        return m_doubleMap[key];
    }
#ifdef DEBUG
    else if (!read && def == 0.0)
    {
        qDebug("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return number;
}


// -------------------------------------------------------------------------------------------------
bool Settings::readBoolEntry(const QString & key, bool def) const
// -------------------------------------------------------------------------------------------------
{
    bool read = false;
    bool res = m_qSettings.readBoolEntry(key, def, &read);
    if (!read && m_boolMap.contains(key))
    {
        return m_boolMap[key];
    }
#ifdef DEBUG
    else if (!read && !def)
    {
        qDebug("Implicit default returned, key = %s", key.latin1());
    }
#endif
    return res;
}

