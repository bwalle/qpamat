/*
 * Id: $Id: timerstatusmessage.h,v 1.1 2003/12/15 18:37:28 bwalle Exp $
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
 * This class auto-deletes itself. Don't delete it and create it only with new, not on
 * the stack! All other doesn't make sense.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/15 18:37:28 $
 */
class TimerStatusmessage : public QObject
{
    Q_OBJECT
        
    public:
        
        /*!
         * Creates a new TimerStatusmessage object. 
         * \param message the message that should be displayed on the statusbar
         * \param time the time while the message is displayed
         * \param statusbar the statusbar on which the message should be displayed
         */
        TimerStatusmessage(const QString& message, int time, QStatusBar* statusbar);
        
    private slots:
        void displayAgain();
        void deleteMyself();
        
    private:
        QStatusBar* m_statusBar;
        int m_time;
        const QString m_message;
        QTime m_begin;
};

#endif // TIMERSTATUSMESSAGE_H
