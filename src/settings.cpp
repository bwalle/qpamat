/*
 * Id: $Id: settings.cpp,v 1.4 2003/12/04 14:08:08 bwalle Exp $
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

#include "settings.h"
#include "security/encryptor.h"

// -------------------------------------------------------------------------------------------------
// Defaults
// -------------------------------------------------------------------------------------------------
const QString Settings::DEFAULT_WEBBROWSER          = "mozilla";
const QString Settings::QPAMAT_FILE_NAME            = QDir::home().absPath()+"/.qpamat";
const int     Settings::DEFAULT_PORT                = 1;
const bool    Settings::DEFAULT_USE_CARD            = false;
const QString Settings::DEFAULT_AUTOTEXT_MISC       = "";
const QString Settings::DEFAULT_AUTOTEXT_USERNAME   = "Username";
const QString Settings::DEFAULT_AUTOTEXT_PASSWORD   = "Password";
const QString Settings::DEFAULT_AUTOTEXT_URL        = "URL";

// -------------------------------------------------------------------------------------------------
Settings::Settings()
// -------------------------------------------------------------------------------------------------
{
    m_qSettings.setPath( "bwalle.de", "qpamat", QSettings::User );
    m_qSettings.beginGroup("/qpamat");
}


// -------------------------------------------------------------------------------------------------
Settings& Settings::getInstance()
// -------------------------------------------------------------------------------------------------
{
    static Settings settings;
    return settings;
}


// -------------------------------------------------------------------------------------------------
QSettings& Settings::getSettings()
// -------------------------------------------------------------------------------------------------
{
    return m_qSettings;
}

