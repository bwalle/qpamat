/*
 * Id: $Id: settings.h,v 1.8 2003/12/20 15:58:02 bwalle Exp $
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
#include <qmap.h>

/*!
 * \brief Singleton for storing settings in registry (MS Windows) or ini-file (Unix).
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.8 $
 * \date $Date: 2003/12/20 15:58:02 $
 */
class Settings
{
    public:
        
        /*! 
         * Creates a new instance of the settings object. The default values are initialized.
         */
        Settings();
        
        /*!
         * Destructor
         */
        virtual ~Settings() { }
        
        /*!
         * Writes an entry into the settings.
         * \param key the key
         * \param value the value
         * \return if the entry was written
         */
        bool writeEntry(const QString & key, bool value);
        
        /*!
         * Writes an entry into the settings.
         * \param key the key
         * \param value the value
         * \return if the entry was written
         */
        bool writeEntry(const QString & key, double value);
        
        /*!
         * Writes an entry into the settings.
         * \param key the key
         * \param value the value
         * \return if the entry was written
         */
        bool writeEntry(const QString & key, int value);
        
        /*!
         * Writes an entry into the settings.
         * \param key the key
         * \param value the value
         * \return if the entry was written
         */
        bool writeEntry(const QString & key, const QString & value);
        
        /*!
         * Reads the entry. If the entry does not exist there's a automatic default value that
         * is returned.
         * \return the entry
         */
        QString readEntry(const QString & key, const QString& def = QString::null);
        
        /*!
         * Reads a number entry. If the entry does not exist there's a automatic default value that
         * is returned.
         * \return the entry
         */
        int readNumEntry (const QString & key, int def = 0);
        
        /*!
         * Reads a double entry. If the entry does not exist there's a automatic default value that
         * is returned.
         * \return the entry
         */
        double readDoubleEntry(const QString & key, double def = 0.0) const;
        
        /*!
         * Reads a boolean entry. If the entry does not exist there's a automatic default value that
         * is returned.
         * \return the entry
         */
        bool readBoolEntry(const QString & key, bool def = false) const;
        
    private:
        QSettings m_qSettings;
        QMap<QString, QString> m_stringMap;
        QMap<QString, int> m_intMap;
        QMap<QString, bool> m_boolMap;
        QMap<QString, double> m_doubleMap;
};

#endif // SETTINGS_H
