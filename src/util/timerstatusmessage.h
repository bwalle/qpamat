/*
 * Id: $Id: timerstatusmessage.h,v 1.2 2003/12/15 21:20:32 bwalle Exp $
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

/**
 * \brief Class for displaying a status message for a fixed amount of time.
 *
 * This class makes it possible to display a status message for a fixed amount of time.
 * If another class changes the status text, this class re-changes it to the old value.
 * Of course this is a hack, but it works.
 *
 * Create a new instance and set the statusbar. Then call the message() method each time
 * you want to display the message.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/15 21:20:32 $
 */
class TimerStatusmessage : public QObject
{
    Q_OBJECT
        
    public:
        
        /*!
         * Creates a new TimerStatusmessage object. 
         * \param statusbar the statusbar on which the message should be displayed
         */
        TimerStatusmessage(QStatusBar* statusbar);
        
    public slots:
        
        /*!
         * Displays a message immediately.
         * \param message the message that should be displayed on the statusbar
         * \param time the time while the message is displayed
         */
        void message(const QString& message, int time);
        
    private slots:
        void displayAgain();
        void disconnectSignalsAndSlots();
        
    private:
        QStatusBar* m_statusBar;
        int m_time;
        QString m_message;
        QTime m_begin;
        bool m_connected;
};

#endif // TIMERSTATUSMESSAGE_H
