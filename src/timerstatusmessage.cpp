/*
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
#include <QString>
#include <QStatusBar>
#include <QDateTime>
#include <QTimer>

#include "timerstatusmessage.h"

/*!
    \class TimerStatusmessage
    
    \brief Class for displaying a status message for a fixed amount of time.

    This class makes it possible to display a status message for a fixed amount of time.
    If another class changes the status text, this class re-changes it to the old value.
    Of course this is a hack, but it works.
    
    Create a new instance and set the statusbar. Then call the message() method each time
    you want to display the message.
    
    \ingroup gui
    \author Bernhard Walle
*/

/*!
    Creates a new TimerStatusmessage object. 
    \param statusbar the statusbar on which the message should be displayed
*/
TimerStatusmessage::TimerStatusmessage(QStatusBar* statusbar)
    : m_statusBar(statusbar), m_connected(false)
{}


/*!
    Displays a message immediately.
    \param message the message that should be displayed on the statusbar
    \param time the time while the message is displayed
*/
void TimerStatusmessage::message(const QString& message, int time)
{
    m_message = message;
    m_begin = QTime::currentTime();
    m_time = time;
    m_statusBar->blockSignals(true);
    m_statusBar->message(m_message, time);
    m_statusBar->blockSignals(false);
    if (!m_connected)
    {
        connect(m_statusBar, SIGNAL(messageChanged(const QString&)), SLOT(displayAgain()));
        m_connected = true;
        QTimer::singleShot(time+1000, this, SLOT(disconnectSignalsAndSlots()));
    }
}


/*!
    Displays the same message again.
*/
void TimerStatusmessage::displayAgain()
{
    int diff = m_begin.msecsTo(QTime::currentTime());
    if (diff < m_time)
    {
        m_statusBar->blockSignals(true);
        m_statusBar->message(m_message, m_time - diff);
        m_statusBar->blockSignals(false);
    }
}


/*!
    Disconnects signals and slots after the timer timed out.
*/
void TimerStatusmessage::disconnectSignalsAndSlots()
{
    disconnect(m_statusBar, SIGNAL(messageChanged(const QString&)), this, SLOT(displayAgain()));
    m_connected = false;
}

// :maxLineLen=100:
