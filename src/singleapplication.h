/*
 * Id: $Id: singleapplication.h,v 1.1 2003/12/15 18:38:39 bwalle Exp $
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
#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <stdexcept>
#include <qobject.h>
#include <qstring.h>

/**
 * \brief This class provides support for a single application.
 *
 * At first you create a SingleApplication object on the stack or on the heap. Then you call
 * the startup() function and connect the shutdown() slot to the aboutToQuit() signal of
 * QApplication.
 *
 * \ingroup misc
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/15 18:38:39 $
 */
class SingleApplication : public QObject
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instnace of SingleApplication.
         * \param lockfileDir the directory in which the lockfile should be created.
         * \param appName the name of the application.
         * \exception std::invalid_argument if the specified direcory does not exit
         */
        SingleApplication(const QString& lockfileDir, const QString& appName) 
            throw (std::invalid_argument);
        
        /*!
         * Deletes a SingleApplication object.
         */
        virtual ~SingleApplication() { };
        
    public slots:
        
        /*!
         * This slot should be called on startup. It creates the logfile and displayes a
         * (global) error dialog if creating the lockfile fails.
         */
        void startup();
        
        /*!
         * This slot should be called on shutdown. It creates the logfile and displayes a
         * (global) error dialog if creating the lockfile fails.
         */
        void shutdown();
        
    private:
        QString m_lockfile;
        const QString m_appName;
};

#endif // SINGLEAPPLICATION_H
