/*
 * Id: $Id: singleapplication.h,v 1.1 2003/12/29 00:13:48 bwalle Exp $
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

class SingleApplication : public QObject
{
    Q_OBJECT
    
    public:
        SingleApplication(const QString& lockfileDir, const QString& appName) 
            throw (std::invalid_argument);
        
    public slots:
        void startup();
        void shutdown();
        
    private:
        QString         m_lockfile;
        const QString   m_appName;
        bool            m_shutdown;
};

#endif // SINGLEAPPLICATION_H
