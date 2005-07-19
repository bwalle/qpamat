/*
 * Id: $Id$
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
#ifndef DOCKTIMEOUTAPPLICATION_H
#define DOCKTIMEOUTAPPLICATION_H

#include <qobject.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qevent.h>



#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#endif   

class DockTimeoutApplication : public QApplication
{
    Q_OBJECT
    Q_PROPERTY( int timeout READ getTimeout WRITE setTimeout )
    Q_PROPERTY( bool temporaryDisabled READ isTemporaryDisabled WRITE setTemporaryDisabled )
    
    public:
        DockTimeoutApplication(int& argc, char** argv);
        DockTimeoutApplication(int& argc, char** argv, bool guiEnabled);
        
    public:
        void setTimeout(int newTimeout);
        int getTimeout() const;
        
        bool isTemporaryDisabled() const;
        void setTemporaryDisabled(bool disabled);
        
    signals:
        void timedOut();
        void dockActivated();
        void newTrayOwner();
        void trayOwnerDied();
        
    protected:
        bool notify(QObject* receiver, QEvent* e);
#ifdef Q_WS_X11
        bool x11EventFilter(XEvent *event);
#endif
#ifdef Q_WS_MAC
        bool macEventFilter(EventHandlerCallRef, EventRef);
#endif

    private:
        void init();
        
    private:
        int m_timeout;
        QTimer* m_timer;
        bool m_temporaryDisabled;
};
            
 
#endif /* DOCKTIMEOUTAPPLICATION_H */
