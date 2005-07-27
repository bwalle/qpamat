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
#include <QWidget>
#ifdef Q_WS_X11
#  include <QX11Info>
#endif

#include "windowfunctions.h"

#ifdef Q_WS_X11
#include<X11/Xlib.h>
#include<X11/Xutil.h> // needed for WM_CLASS hinting

void x11wmClass(Display *dsp, WId wid, QString resName)
{
	char app_name[] = "psi";

	//Display *dsp = x11Display();			// get the display
	//WId win = winId();				// get the window
	XClassHint classhint;				// class hints
	classhint.res_name = (char *)resName.latin1();	// res_name
	classhint.res_class = app_name;			// res_class
	XSetClassHint(dsp, wid, &classhint); 		// set the class hints
}

//>>>-- Nathaniel Gray -- Caltech Computer Science ------>
//>>>-- Mojave Project -- http://mojave.cs.caltech.edu -->
// Copied from http://www.nedit.org/archives/discuss/2002-Aug/0386.html

// Helper function
bool getCardinal32Prop(Display *display, Window win, char *propName, long *value)
{
	Atom nameAtom, typeAtom, actual_type_return;
	int actual_format_return, result;
	unsigned long nitems_return, bytes_after_return;
	long *result_array=NULL;

	nameAtom = XInternAtom(display, propName, False);
	typeAtom = XInternAtom(display, "CARDINAL", False);
	if (nameAtom == None || typeAtom == None) {
		//qDebug("Atoms not interned!");
		return false;
	}


	// Try to get the property
	result = XGetWindowProperty(display, win, nameAtom, 0, 1, False,
		typeAtom, &actual_type_return, &actual_format_return,
		&nitems_return, &bytes_after_return,
		(unsigned char **)&result_array);

	if( result != Success ) {
		//qDebug("not Success");
		return false;
	}
	if( actual_type_return == None || actual_format_return == 0 ) {
		//qDebug("Prop not found");
		return false;
	}
	if( actual_type_return != typeAtom ) {
		//qDebug("Wrong type atom");
	}
	*value = result_array[0];
	XFree(result_array);
	return true;
}


// Get the desktop number that a window is on
bool desktopOfWindow(Window *window, long *desktop)
{
	Display *display = QX11Info::display();
	bool result = getCardinal32Prop(display, *window, (char *)"_NET_WM_DESKTOP", desktop);
	//if( result )
	//	qDebug("Desktop: " + QString::number(*desktop));
	return result;
}


// Get the current desktop the WM is displaying
bool currentDesktop(long *desktop)
{
	Window rootWin;
	Display *display = QX11Info::display();
	bool result;

	rootWin = RootWindow(QX11Info::display(), XDefaultScreen(QX11Info::display()));
	result = getCardinal32Prop(display, rootWin, (char *)"_NET_CURRENT_DESKTOP", desktop);
	//if( result )
	//	qDebug("Current Desktop: " + QString::number(*desktop));
	return result;
}
#endif



/*!
    \class WindowFunctions
    
    \brief Some functions for manipulating windows.
    
    Some functions for manipulating windows. The functions are static, so no instance of this
    class has to be created
    
    \ingroup misc
    \author Bernhard Walle
    \version $Revision: 1.1 $
    \date $Date$
*/


/*!
    Brings a window to the front.

    \param w the window widget
*/
void WindowFunctions::bringToFront(QWidget* w)
{
    #ifdef Q_WS_X11
    // If we're not on the current desktop, do the hide/show trick
    long dsk, curr_dsk;
    Window win = w->winId();
    if(desktopOfWindow(&win, &dsk) && currentDesktop(&curr_dsk)) 
    {
        if(dsk != curr_dsk) 
        {
            w->hide();
            //qApp->processEvents();
        }
    }
    
    // FIXME: multi-desktop hacks for Win and Mac required
#endif

    w->show();
    if(w->isMinimized()) 
    {
        //w->hide();
        if(w->isMaximized())
        {
            w->showMaximized();
        }
        else
        {
            w->showNormal();
        }
    }
    //if(grabFocus)
    //      w->setActiveWindow();
    w->raise();
    w->setActiveWindow();
}

