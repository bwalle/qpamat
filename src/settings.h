/*
 * Id: $Id: settings.h,v 1.7 2003/12/06 18:23:07 bwalle Exp $
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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <qsettings.h>

/*!
 * \brief Singleton for storing settings in registry (MS Windows) or ini-file (Unix).
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.7 $
 * \date $Date: 2003/12/06 18:23:07 $
 */
class Settings
{
    public:
        
        /*! the default web browser */
        static const QString DEFAULT_WEBBROWSER;
        
        /*! the default file name */
        static const QString QPAMAT_FILE_NAME;
        
        /*! the default port number */
        static const int DEFAULT_PORT;
        
        /*! if the card should be used */
        static const bool DEFAULT_USE_CARD;
        
        /*! default for auto text: Misc */
        static const QString DEFAULT_AUTOTEXT_MISC;
        
        /*! default for auto text: Username */
        static const QString DEFAULT_AUTOTEXT_USERNAME;
        
        /*! default for auto text: Password */
        static const QString DEFAULT_AUTOTEXT_PASSWORD;
        
        /*! default for auto text: URL */
        static const QString DEFAULT_AUTOTEXT_URL;
        
        /*! default sans-serif font */
        static const QString DEFAULT_NORMAL_FONT;
        
        /*! default serif font */
        static const QString DEFAULT_FOOTER_FONT;
        
        /*!
         * Destructor
         */
        virtual ~Settings() { }
        
        /*!
         * Retuns the only one instance of the Settings object
         * \return the instance
         */
        static Settings& getInstance();
        
        /*!
         * Returns the QSettings object, used for writing and reading simple keys
         * \return the object
         */
        QSettings& getSettings();
        
    private:
        Settings();
        QSettings m_qSettings;
};

#endif // SETTINGS_H
