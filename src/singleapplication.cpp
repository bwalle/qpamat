/*
 * Id: $Id: singleapplication.cpp,v 1.1 2003/12/15 18:38:39 bwalle Exp $
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
#include <stdexcept>

#ifdef Q_WS_X11
#include <unistd.h>
#endif

#include <qobject.h>
#include <qdir.h>
#include <qmessagebox.h>

#include "singleapplication.h"

// -------------------------------------------------------------------------------------------------
SingleApplication::SingleApplication(const QString& lockfileDir, const QString& appName)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument) 
          : QObject(0, "SingleApplication"), m_appName(appName)
{
    if (!QDir(lockfileDir).isReadable())
    {
        throw std::invalid_argument( QString("The directory %1 does not exist.").arg(lockfileDir)
            .latin1());
    }
    m_lockfile = lockfileDir + "/" + "." + appName.lower() + ".lock";
}


// -------------------------------------------------------------------------------------------------
void SingleApplication::startup()
// -------------------------------------------------------------------------------------------------
{
    QFile file(m_lockfile);
    
    // check if the file exists
    if (QFile::exists(m_lockfile))
    {
        QString id = "???";
        if (file.open(IO_ReadOnly))
        {
            QTextStream textstream(&file);
            textstream >> id;
        }
        
        QMessageBox::critical(0, m_appName, QObject::tr("You can only start one instance of "
            "%1. If you are\nreally sure that no other instance is running, "
            "delete\nthe file %2 and start again."
#ifdef Q_WS_X11
            "\n\n(PID of the other %1 process should be %3.)"
#endif
            ).arg(m_appName).arg(m_lockfile)
#ifdef Q_WS_X11
            .arg(id)
#endif
            , QMessageBox::Ok, QMessageBox::NoButton);
        std::exit(1);
    }
    
    
    if (!file.open(IO_WriteOnly))
    {
        qDebug("Could not open the file %s for writing.", m_lockfile.latin1());
        return;
    }
    QTextStream textstream(&file);
#ifdef Q_WS_X11
    textstream << getpid();
#else
    textstream << "???.";
#endif
}


// -------------------------------------------------------------------------------------------------
void SingleApplication::shutdown()
// -------------------------------------------------------------------------------------------------
{
    if (!QFile::remove(m_lockfile))
    {
        qDebug("Could not remove the lockfile %s", m_lockfile.latin1());
    }
}
