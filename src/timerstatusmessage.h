/*
 * Id: $Id: timerstatusmessage.h,v 1.1 2003/12/29 15:12:41 bwalle Exp $
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
#ifndef TIMERSTATUSMESSAGE_H
#define TIMERSTATUSMESSAGE_H

#include <qstring.h>
#include <qstatusbar.h>
#include <qdatetime.h>

class TimerStatusmessage : public QObject
{
    Q_OBJECT
        
    public:
        TimerStatusmessage(QStatusBar* statusbar);
        
    public slots:
        void message(const QString& message, int time);
        
    private slots:
        void displayAgain();
        void disconnectSignalsAndSlots();
        
    private:
        QStatusBar* m_statusBar;
        int         m_time;
        QString     m_message;
        QTime       m_begin;
        bool        m_connected;
};

#endif // TIMERSTATUSMESSAGE_H
