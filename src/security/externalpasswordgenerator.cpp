/*
 * Id: $Id: externalpasswordgenerator.cpp,v 1.3 2003/12/29 10:59:16 bwalle Exp $
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
#include <qprocess.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qeventloop.h>

#include "global.h"
#include "externalpasswordgenerator.h"

// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

/*!
    The timeout for the application call.
*/
const int ExternalPasswordGenerator::TIMEOUT = 5*1000;


/*!
    \class ExternalPasswordGenerator
    
    \brief Object to generate random passwords. 

    Generates passwords using a random application. The application gets the required length
    as first argument. A shellscript wrapper may be used.
    
    \ingroup security
    \author Bernhard Walle
    \version $Revision: 1.3 $
    \date $Date: 2003/12/29 10:59:16 $
*/

/*!
    Creates a new instance of ExternalPasswordGenerator. 
    \param applicationName the name of the application which should be called to get the password
*/
ExternalPasswordGenerator::ExternalPasswordGenerator(const QString& applicationName)
    : m_applicationName(applicationName)
{}


/*!
    Generates a random password.
    \param length the length of the password
    \param allowedChars ignored
    \return the password
    \exception PasswordGenerateException if launching the external application failed
*/
QString ExternalPasswordGenerator::getPassword(uint length, const QString&)
        throw (PasswordGenerateException)
{
    if (m_applicationName.isNull())
    {
        throw PasswordGenerateException("In ExternalPasswordGenerator::getPassword: No program set."); 
    }
    QProcess* proc = new QProcess(0);
    proc->setArguments(QStringList::split(" ", m_applicationName));
    proc->addArgument(QString::number(length));
    if (!proc->start())
    {
        delete proc;
        throw PasswordGenerateException( QString("Error while launching the external program \"%1\"")
            .arg(m_applicationName).latin1() );
    }
    QTimer::singleShot( 5*1000, proc, SLOT(kill()) );
    while (proc->isRunning())
    {
        qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::WaitForMore);
    }
    if (!proc->normalExit())
    {
        delete proc;
        throw PasswordGenerateException( QString("Program exited not normally (exitcode %1)")
            .arg(QString::number(proc->exitStatus())).latin1() );
    }
    QString returnvalue = proc->readLineStdout();
    delete proc;
    return returnvalue;
}


/*!
    Returns \c true
    \return \c true
*/
bool ExternalPasswordGenerator::isSlow() const
{
    return true;
}
