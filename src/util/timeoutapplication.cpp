/*
 * Id: $Id: timeoutapplication.cpp,v 1.1 2005/02/12 10:52:21 bwalle Exp $
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
#include <qtimer.h>
#include <qapplication.h>
#include <qevent.h>

#include "timeoutapplication.h"

/*!
    \class TimeoutApplication
    
    \brief Support for a application that wants to trigger an inactivity action after a timeout
    
    Some applications wants to perform an action after some time of inactivity of the user.
    A user is active if the application receives mouse and/or key events. 
    
    \ingroup misc
    \author Bernhard Walle
    \version $Revision: 1.1 $
    \date $Date: 2005/02/12 10:52:21 $
*/





/*!
    Creates a new TimeoutApplication object. 
    
    See QApplication::QApplication(int&, char**) for details.
    
    \param argc the args of the main() function
    \param argv the argv of the main() function
*/
TimeoutApplication::TimeoutApplication(int& argc, char** argv)
    : QApplication(argc, argv), m_timeout(0), m_timer(0)
{
    init();
}


/*!
    Creates a new TimeoutApplication object. 
    
    See QApplication::QApplication(int&, char**, bool) for details.
    
    \param argc the args of the main() function
    \param argv the argv of the main() function
    \param guiEnabled if it is a GUI application
*/
TimeoutApplication::TimeoutApplication(int& argc, char** argv, bool guiEnabled)
    : QApplication(argc, argv, guiEnabled), m_timeout(0), m_timer(0)
{
    init();
}


/*!
    Common init function because in C++ a constructor cannot call another constructor.
*/
void TimeoutApplication::init()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SIGNAL(timedOut()));
}

/*!
    \property TimeoutApplication::timeout
    
    The inactivity timeout after that the timedOut signal should be triggered. The value
    must be positive, negative values are not permitted.
*/

/*!
    Returns the timeout value in minutes. Zero means the timeout feature is disabled. 
    
    \return the timeout
*/
int TimeoutApplication::getTimeout() const
{
    return m_timeout;
}


/*!
    Sets the timeout. Zero means that the timeout feature is disabled. Negative values are
    not permitted.
    
    \param timeout the timeout in milliseconds
*/
void TimeoutApplication::setTimeout(int timeout)
{
    m_timeout = timeout;
}


/*!
    Overwrites QApplication::nofity(QObject*, QEvent*). Restarts the timer if necessary.
    
    \param receiver the receiver
    \param e the event
*/
bool TimeoutApplication::notify(QObject* receiver, QEvent* e)
{
    if (m_timeout != 0 && 
            (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease
              || e->type() == QEvent::MouseMove || e->type() == QEvent::KeyPress))
    {
        m_timer->start(m_timeout*1000*60);
    }
    
    return QApplication::notify(receiver, e);
}

/*!
    \fn TimeoutApplication::timedOut()
    
    Signal that is emitted if the inactivity timeout occured.
*/

