/*
 * Id: $Id: settings.h,v 1.2 2003/11/04 22:49:38 bwalle Exp $
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

/**
 * Singleton for storing settings in registry (MS Windows) or ini-file (Unix).
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.2 $
 * @date $Date: 2003/11/04 22:49:38 $
 */
class Settings
{
    public:
        
        /** the default web browser */
        static QString DEFAULT_WEBBROWSER;
        
        /** the default file name */
        static QString QPAMAT_FILE_NAME;
        
        /**
         * Destructor
         */
        virtual ~Settings() { }
        
        /**
         * Retuns the only one instance of the Settings object
         * @return the instance
         */
        static Settings& getInstance();
        
        /**
         * Returns the QSettings object, used for writing and reading simple keys
         * @return the object
         */
        QSettings& getSettings();
        
    private:
        Settings();
        QSettings m_qSettings;
};

#endif // SETTINGS_H
