/*
 * Id: $Id: timerstatusmessage.cpp,v 1.1 2003/12/15 18:37:28 bwalle Exp $
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
TimerStatusmessage::TimerStatusmessage(const QString& message, int time, QStatusBar* sb)
// -------------------------------------------------------------------------------------------------
            : m_statusBar(sb), m_time(time), m_message(message)
{
    m_begin = QTime::currentTime();
    m_statusBar->message(m_message, time);
    connect(m_statusBar, SIGNAL(messageChanged(const QString&)), SLOT(displayAgain()));
    connect(m_statusBar, SIGNAL(destroyed()), SLOT(deleteMyself()));
    // delete myself one second
    QTimer::singleShot(time + 1000, this, SLOT(deleteMyself()));
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
void TimerStatusmessage::deleteMyself()
// -------------------------------------------------------------------------------------------------
{
    qDebug("Deleted myself");
    delete this;
}

