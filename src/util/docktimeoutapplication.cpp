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
#include <QTimer>
#include <QApplication>
#include <QList>
#include <QObject>
#include <QEvent>
#ifdef Q_WS_X11
#  include <QX11Info>
#endif
#include <QDesktopWidget>

#include "docktimeoutapplication.h"
#include "global.h"

/*!
    \class DockTimeoutApplication
    
    \brief Support for a application that wants to trigger an inactivity action after a timeout
    
    Some applications wants to perform an action after some time of inactivity of the user.
    A user is active if the application receives mouse and/or key events. 
    
    \ingroup misc
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date$
*/


#ifdef Q_WS_X11
#include <time.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/SM/SMlib.h>

// Atoms required for monitoring the freedesktop.org notification area
static Atom manager_atom = 0;
static Atom tray_selection_atom = 0;
Window root_window = 0;
Window tray_owner = None;

//static Atom atom_KdeNetUserTime;

//#if QT_VERSION > 0x030201
//#warning "Possibly, now it's time to clean up some 'focus stealing prevention' workaround code"
//#endif

#ifdef KeyPress
#ifndef FIXX11H_KeyPress
#define FIXX11H_KeyPress
const int XKeyPress = KeyPress;
#undef KeyPress
const int KeyPress = XKeyPress;
#endif
#undef KeyPress
#endif
#endif

// mblsha:
// currently this file contains some Anti-"focus steling prevention" code by
// Lubos Lunak (l.lunak@kde.org)
//
// This should resolve all bugs with KWin3 and old Qt, but maybe it'll be useful for
// other window managers?

#ifdef Q_WS_X11
//#undef Q_WS_X11
#endif


#ifdef Q_WS_X11
void setTrayOwnerWindow(Display *dsp)
{
	/* This code is basically trying to mirror what happens in
	 * eggtrayicon.c:egg_tray_icon_update_manager_window()
	 */
	// ignore events from the old tray owner
	if (tray_owner != None)
	{
		XSelectInput(dsp, tray_owner, 0);
	}

	// obtain the Window handle for the new tray owner
	XGrabServer(dsp);
	tray_owner = XGetSelectionOwner(dsp, tray_selection_atom);
    
	// we have to be able to spot DestroyNotify messages on the tray owner
	if (tray_owner != None)
	{
		XSelectInput(dsp, tray_owner, StructureNotifyMask|PropertyChangeMask);
	}
	XUngrabServer(dsp);
	XFlush(dsp);
}

#endif


/*!
    Creates a new DockTimeoutApplication object. 
    
    See QApplication::QApplication(int&, char**) for details.
    
    \param argc the args of the main() function
    \param argv the argv of the main() function
*/
DockTimeoutApplication::DockTimeoutApplication(int& argc, char** argv)
    : QApplication(argc, argv), m_timeout(0), m_timer(0), m_temporaryDisabled(false)
{
    init();
}


/*!
    Creates a new DockTimeoutApplication object. 
    
    See QApplication::QApplication(int&, char**, bool) for details.
    
    \param argc the args of the main() function
    \param argv the argv of the main() function
    \param guiEnabled if it is a GUI application
*/
DockTimeoutApplication::DockTimeoutApplication(int& argc, char** argv, bool guiEnabled)
    : QApplication(argc, argv, guiEnabled), m_timeout(0), m_timer(0), m_temporaryDisabled(false)
{
    init();
}


/*!
    Common init function because in C++ a constructor cannot call another constructor.
*/
void DockTimeoutApplication::init()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SIGNAL(timedOut()));
    
#ifdef Q_WS_X11
    const int max = 20;
    Atom* atoms[max];
    char* names[max];
    Atom atoms_return[max];
    int n = 0;

    //atoms[n] = &atom_KdeNetUserTime;
    //names[n++] = (char *) "_KDE_NET_USER_TIME";

    atoms[n] = &manager_atom;
    names[n++] = (char *) "MANAGER";

    Display *dsp = QX11Info::display();

    XInternAtoms( dsp, names, n, false, atoms_return );

    for (int i = 0; i < n; i++ )
        *atoms[i] = atoms_return[i];

    // get the selection type we'll use to locate the notification tray
    char buf[32];
    snprintf(buf, sizeof(buf), "_NET_SYSTEM_TRAY_S%d", 
        XScreenNumberOfScreen( XDefaultScreenOfDisplay(dsp) ));
    tray_selection_atom = XInternAtom(dsp, buf, false);

    // make a note of the window handle for the root window
    root_window = QApplication::desktop()->winId();

    XWindowAttributes attr;

    // this is actually futile, since Qt overrides it at some
    // unknown point in the near future.
    XGetWindowAttributes(dsp, root_window, &attr);
    XSelectInput(dsp, root_window, attr.your_event_mask | StructureNotifyMask);

    setTrayOwnerWindow(dsp);
#endif
}

/*!
    \property DockTimeoutApplication::timeout
    
    The inactivity timeout after that the timedOut signal should be triggered. The value
    must be positive, negative values are not permitted.
*/

/*!
    Returns the timeout value in minutes. Zero means the timeout feature is disabled. 
    
    \return the timeout
*/
int DockTimeoutApplication::getTimeout() const
{
    return m_timeout;
}


/*!
    Sets the timeout. Zero means that the timeout feature is disabled. Negative values are
    not permitted.
    
    \param timeout the timeout in milliseconds
*/
void DockTimeoutApplication::setTimeout(int timeout)
{
    qDebug("timeout = %d", timeout);
    m_timeout = timeout;
}


/*!
    \property DockTimeoutApplication::temporaryDisabled

    Sometimes, the call of the timedOut() function has to disabled temporary without loosing
    the timeout value. To do this, set temporaryDisabled to true.
*/

/*!
    Checks if the DockTimeoutApplication is disabled temporary.
    
    \return \c true if it is disabled, \c false if not
*/
bool DockTimeoutApplication::isTemporaryDisabled() const
{
    return m_temporaryDisabled;
}

/*!
    Sets the timeout function to disabled temporary.
    
    \param disabled \c true if it should be disabled, \c false if not
*/
void DockTimeoutApplication::setTemporaryDisabled(bool disabled)
{
    m_timer->stop();
    m_temporaryDisabled = disabled;
    qDebug("disable = %d", disabled);
}


/*!
    Adds receivers to ignore for timeout. This is a kind of hack to ignore
    events which get passed to the tray icon. If the object is destroyed,
    it does not gets automatically removed from this list. You have to take care
    this yourself!

    \param receiver the receiver to ignore
*/
void DockTimeoutApplication::addReceiverToIgnore(void* receiver)
{
    m_receiversToIgnore.append(receiver);
    qDebug("Addding = %d", receiver);
}


/*!
    
*/
void DockTimeoutApplication::removeReceiverToIgnore(void* receiver)
{
    m_receiversToIgnore.removeAll(receiver);
}


/*!
    Clears the list of receivers to ignore.
*/
void DockTimeoutApplication::clearReceiversToIgnore()
{
    m_receiversToIgnore.clear();
}


/*!
    Overwrites QApplication::nofity(QObject*, QEvent*). Restarts the timer if necessary.
    
    \param receiver the receiver
    \param e the event
*/
bool DockTimeoutApplication::notify(QObject* receiver, QEvent* e)
{
    if (!m_temporaryDisabled && m_timeout != 0 && 
            !m_receiversToIgnore.contains(receiver) &&
            (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease
              || e->type() == QEvent::MouseMove || e->type() == QEvent::KeyPress))
    {
        m_timer->start(m_timeout*1000*60);
        qDebug("Start timer (%d)", e->type());
        qDebug("Receiver = %d", receiver);
        qDebug("Receiver = %d", receiver->name());
    }
    
    return QApplication::notify(receiver, e);
}



/*!
    \fn DockTimeoutApplication::timedOut()
    
    Signal that is emitted if the inactivity timeout occured.
*/

#ifdef Q_WS_X11
bool DockTimeoutApplication::x11EventFilter( XEvent *_event )
{
	switch ( _event->type ) {

		case ClientMessage:
			if (_event->xclient.window == root_window && _event->xclient.message_type == manager_atom)
			{
				// A new notification area application has
				// announced its presence
				setTrayOwnerWindow(_event->xclient.display);
				emit newTrayOwner();
                qDebug("new tray owner");
				break;
			}
		case DestroyNotify:
			if (_event->xdestroywindow.event == tray_owner)
			{
				// there is now no known notification area.
				// We're still looking out for the MANAGER
				// message sent to the root window, at which
				// point we'll have another look to see
				// whether a notification area is available.
				tray_owner = 0;
				emit trayOwnerDied();
				break;
			}
            break;

		default:
			break;
	}

	// process the event normally
	return false;
}
#endif


#ifdef Q_WS_MAC
bool DockTimeoutApplication::macEventFilter(EventHandlerCallRef, EventRef inEvent)
{
	UInt32 eclass = GetEventClass(inEvent);
	int etype = GetEventKind(inEvent);
	
    if (eclass == 'eppc' && etype == kEventAppleEvent) 
    {
		dockActivated();
	}
    
	return false;
}
#endif


/*!
    \fn DockTimeoutApplication::dockActivated
    
    Emitted if the dock was activated. Don't know really what this means, function is from
    Psi.
*/

/*!
    \fn DockTimeoutApplication::newTrayOwner()
    
    Emitted if the system tray has a new owner. Only occures on X11.
*/

/*!
    \fn DockTimeoutApplication::trayOwnerDied
    
    Emitted if the tray owner died. Only occures on X11.
*/
