/*
 * Id: $Id: timerstatusmessage.cpp,v 1.3 2003/12/17 23:24:32 bwalle Exp $
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
#include <qstatusbar.h>
#include <qdatetime.h>
#include <qtimer.h>

#include "timerstatusmessage.h"

// -------------------------------------------------------------------------------------------------
TimerStatusmessage::TimerStatusmessage(QStatusBar* sb)
// -------------------------------------------------------------------------------------------------
            : m_statusBar(sb), m_connected(false)
{}


// -------------------------------------------------------------------------------------------------
void TimerStatusmessage::message(const QString& message, int time)
// -------------------------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------------------------
void TimerStatusmessage::displayAgain()
// -------------------------------------------------------------------------------------------------
{
    int diff = m_begin.msecsTo(QTime::currentTime());
    if (diff < m_time)
    {
        m_statusBar->blockSignals(true);
        m_statusBar->message(m_message, m_time - diff);
        m_statusBar->blockSignals(false);
    }
}


// -------------------------------------------------------------------------------------------------
void TimerStatusmessage::disconnectSignalsAndSlots()
// -------------------------------------------------------------------------------------------------
{
    disconnect(m_statusBar, SIGNAL(messageChanged(const QString&)), this, SLOT(displayAgain()));
    m_connected = false;
}

