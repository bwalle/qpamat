/*
 * Id: $Id: singleapplication.cpp,v 1.4 2004/01/11 23:19:52 bwalle Exp $
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
#include <csignal>

#ifndef Q_WS_WIN
#include <unistd.h>
#else
#include <process.h>
#endif

#include <qobject.h>
#include <qdir.h>
#include <qmessagebox.h>

#include "global.h"
#include "singleapplication.h"

QString SingleApplication::lockfile;
QString SingleApplication::appName;
bool    SingleApplication::didShutdownAlready;
bool    SingleApplication::initialized;

/*!
    \class SingleApplication
    
    \brief This class provides support for a single application.
    
    This class helps to ensure that the application can be only started once (this means
    normally once as user but this depends on the directory). Normally, you take the
    user's home directory as lock directory.
    
    I choosed a design with static methods because we need to register the members as
    signal handlers etc. and this is only possible with static members.
    
    At first, you have to initialize with SingleApplication::init(). You have to choose
    the direcory and the application name. This is to determine the file name. If you call
    the SingleApplication::startup() method, a lockfile is created and the process id is written 
    in this lockfile.
    
    Then you have to register the SingleApplication::shutdown() method as exit handler, use
    the function atexit() for this. It's a standard C function. This function gets called on
    normal exit and deletes the lockfile.
    
    If the application crashes or the user presses Ctrl-C on a terminal or sends the kill signal,
    the application receives a signal. So you have to register the SingleApplication::shutdown(int)
    method as singal handler. 
    
    There's a function SingleApplication::registerStandardExitHandlers() which does this for you.
    
    \ingroup misc
    \author Bernhard Walle
    \version $Revision: 1.4 $
    \date $Date: 2004/01/11 23:19:52 $
*/

/*!
    Creates a new instnace of SingleApplication. It checks the existence of the directory
    but does not create any lockfile. The name of the file will be 
    <tt>\<dir\>/.\<applName\>.lock</tt>, so it's hidden on Unix.
    \param lockfileDir the directory in which the lockfile should be created.
    \param applName the name of the application.
    \exception std::invalid_argument if the specified direcory does not exit
*/

void SingleApplication::init(const QString& lockfileDir, const QString& applName) 
    throw (std::invalid_argument)
{
    if (!QDir(lockfileDir).isReadable())
    {
        throw std::invalid_argument( QString("The directory %1 does not exist.").arg(lockfileDir)
            .latin1());
    }
    lockfile = lockfileDir + "/" + "." + applName.lower() + ".lock";
    SingleApplication::appName = applName;
    initialized = true;
}
    
/*!
    This function should be called on startup. It creates the logfile and displayes a (global) 
    error dialog if creating the lockfile fails.
*/
void SingleApplication::startup()
{
    QFile file(lockfile);
    
    // check if the file exists
    if (QFile::exists(lockfile))
    {
        QString id = "???";
        if (file.open(IO_ReadOnly))
        {
            QTextStream textstream(&file);
            textstream >> id;
        }
        
        QMessageBox::critical(0, appName, QObject::tr("You can only start one instance of "
            "%1. If you are\nreally sure that no other instance is running, delete\nthe file %2 "
            "and start again.\n\n(PID of the other %3 process should be %4.)").arg(appName).arg(
            lockfile).arg(appName).arg(id), QMessageBox::Ok, QMessageBox::NoButton);
        std::exit(1);
    }
    
    
    if (!file.open(IO_WriteOnly))
    {
        PRINT_DBG("Could not open the file %s for writing.", lockfile.latin1());
        return;
    }
    QTextStream textstream(&file);
#ifndef Q_WS_WIN
    textstream << getpid();
#else
    textstream << _getpid();
#endif
}


/*!
    This function registers the SingleApplication::shutdown(int) function as exit handler for
    the \c SIGINT, \c SIGTERM, \c SIGABRT signal on any operating system and for the
    \c SIGHUP and \c SIGQUIT signal on Unix and MacOS X.
*/
void SingleApplication::registerStandardExitHandlers()
{
    std::signal(SIGINT, SingleApplication::shutdown);
    std::signal(SIGTERM, SingleApplication::shutdown);
    std::signal(SIGABRT, SingleApplication::shutdown);
#ifndef Q_WS_WIN
    std::signal(SIGHUP, SingleApplication::shutdown);
    std::signal(SIGQUIT, SingleApplication::shutdown);
#endif 
    std::atexit(SingleApplication::shutdown);
}


/*!
    This is the signal handler that should be registered with signal(). The function just exists
    the program normally with the error code 1. If the signal is \c SIGABRT or \c SIGQUIT, 
    a coredump is created, i.e. it sets the default signal handler and calls abort().
    \param signal the signal number
*/
void SingleApplication::shutdown(int signal)
{
    PRINT_DBG("Caught signal %d", signal)
    
    if (signal == SIGABRT || signal == SIGQUIT)
    {
        shutdown();
        std::signal(SIGABRT, SIG_DFL);
        std::abort();
    }
    else
    {
        std::exit(1);
    }
}


/*!
    This function deletes the lockfile. Multiple calls of this function don't harm.
*/
void SingleApplication::shutdown()
{
    PRINT_DBG("Shutting down ...");
    if (!didShutdownAlready) // prevents multiple calls
    {
        if (!QFile::remove(lockfile))
        {
            PRINT_DBG("Could not remove the lockfile %s", lockfile.latin1());
        }
        didShutdownAlready = true;
    }
}
