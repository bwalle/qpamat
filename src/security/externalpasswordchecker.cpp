/*
 * Id: $Id: externalpasswordchecker.cpp,v 1.2 2003/12/17 21:55:53 bwalle Exp $
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
#include <qstring.h>
#include <cstdlib>

#include <qprocess.h>
#include <qapplication.h>
#include <qeventloop.h>
#include <qtimer.h>

#include "externalpasswordchecker.h"

// -------------------------------------------------------------------------------------------------
const int ExternalPasswordChecker::TIMEOUT = 5*1000;
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
ExternalPasswordChecker::ExternalPasswordChecker(const QString& applicationName) 
// -------------------------------------------------------------------------------------------------
        : m_programName(applicationName)
{}


// -------------------------------------------------------------------------------------------------
bool ExternalPasswordChecker::isPasswordOk(const QString& password) throw (PasswordCheckException)
// -------------------------------------------------------------------------------------------------
{
    if (m_programName.isNull())
    {
        throw PasswordCheckException( "In ExternalPasswordChecker::isPasswordOk: No program set."); 
    }
    
    QProcess* proc = new QProcess(0);
    proc->setArguments(QStringList::split(" ", m_programName));
    if (!proc->launch(password))
    {
        delete proc;
        throw PasswordCheckException( QString("Error while launching the external program \"%1\"")
            .arg(m_programName).latin1() );
    }
    QTimer::singleShot( 5*1000, proc, SLOT(kill()) );
    while (proc->isRunning())
    {
        qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::WaitForMore);
    }
    bool normalExit = proc->normalExit();
    int exitStatus = proc->exitStatus();
    delete proc;
    
    return normalExit && exitStatus == EXIT_SUCCESS;
}


// -------------------------------------------------------------------------------------------------
uint ExternalPasswordChecker::minimalLength() const
// -------------------------------------------------------------------------------------------------
{
    return 0;
}


// -------------------------------------------------------------------------------------------------
bool ExternalPasswordChecker::isSlow() const
// -------------------------------------------------------------------------------------------------
{
    return true;
}
