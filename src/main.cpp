/*
 * Id: $Id: main.cpp,v 1.12 2003/12/16 22:53:42 bwalle Exp $
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
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <qapplication.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qdesktopwidget.h>
#include <qsettings.h>
#include <qfile.h>
#include <qdir.h>

#include "qpamat.h"
#include "settings.h"
#include "main.h"
#include "singleapplication.h"

#ifdef Q_WS_X11
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#endif


Qpamat* qpamat;


// -------------------------------------------------------------------------------------------------
void qpamatNS::printCommandlineOptions()
// -------------------------------------------------------------------------------------------------
{
    std::cerr 
        << "\n" 
        << "This is QPaMaT " << VERSION << ", a password managing tool for Unix, MacOS X\n"
        << "and Windows using the Qt programming library from Trolltech.\n\n"
        << "Options: -h            prints this help\n"
        << "         -style ...    specifies the Qt style, e.g. \"platinum\" or \"sgi\"\n"
#ifdef Q_WS_X11
        << "         -display ...  specifies the display for the X Window System\n"
        << "         -fn ...       specifies the font in XLFD syntax for text elements\n"
        << "         -bg ...       specifies the background color (examples: green, #FFFFFF)\n"
        << "         -fg ...       specifies the foreground color\n"
#endif
        << std::endl;
}


// -------------------------------------------------------------------------------------------------
void qpamatNS::printVersion()
// -------------------------------------------------------------------------------------------------
{
#ifdef Q_WS_X11
    struct utsname uname_struct;
    bool nameAvailable = (uname(&uname_struct) >= 0);
    QString X11protocolVersion, X11vendorVersion;
    getX11Version(X11protocolVersion, X11vendorVersion);
#endif

    std::cerr 
        << "QPaMaT version   " << VERSION << "\n"
        << "\nBased on software of this version: \n"
        << "  Qt version     " << QT_VERSION_STR << "\n"
#ifdef Q_WS_X11
        << "  OS name        " << (nameAvailable ? uname_struct.sysname : "unknown") << "\n"
        << "  OS release     " << (nameAvailable ? uname_struct.release : "unknown") << "\n"
        << "  OS version     " << (nameAvailable ? uname_struct.version : "unknown") << "\n"
        << "  OS machine     " << (nameAvailable ? uname_struct.machine : "unknown") << "\n"
        << "  X11 Protocol   " << X11protocolVersion << "\n"
        << "  X11 Release    " << X11vendorVersion
#endif
        << std::endl;
}


// -------------------------------------------------------------------------------------------------
void qpamatNS::parseCommandLine(int& argc, char**& argv)
// -------------------------------------------------------------------------------------------------
{
    for (int i = 1; i < argc; ++i)
    {
        QString string = QString::fromLatin1(argv[i]);
        if (string == "-h" || string == "--help" || string == "-help")
        {
            printCommandlineOptions();
            std::exit(0);
        }
        else if (string == "v" || string == "--version" || string == "-version")
        {
            printVersion();
            std::exit(0);
        }
    }
}


// -------------------------------------------------------------------------------------------------
void qpamatNS::getX11Version(QString& protocolVersion, QString& vendorVersion)
// -------------------------------------------------------------------------------------------------
{
#ifdef Q_WS_X11
    Display* dpy = XOpenDisplay(0);
    if (dpy)
    {
        protocolVersion = QString("%1.%2").arg(QString::number(ProtocolVersion (dpy))).
            arg(ProtocolRevision (dpy));
        vendorVersion = QString::number(VendorRelease (dpy));
        XCloseDisplay(dpy);
    }
#endif
}


// -------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
// -------------------------------------------------------------------------------------------------
{
    int returncode = 0;
    
    QApplication app(argc, argv);
    SingleApplication single(QDir::homeDirPath(), "QPaMaT");
    qpamatNS::parseCommandLine(argc, argv);
    
    try
    {
        single.startup();
        
        qpamat = new Qpamat();
        app.setMainWidget(qpamat);
        
        qpamat->show();
        app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
        app.connect(&app, SIGNAL(aboutToQuit()), &single, SLOT(shutdown())); 
        
        returncode = app.exec();
    }
    catch (const std::bad_alloc& e)
    {
        QMessageBox::warning(0, QObject::tr("QPaMaT"), 
            QObject::tr("No more memory available. The application\nwill be closed."),
            QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton); 
    }
    catch (const std::exception& e)
    {
        QMessageBox::warning(0, QObject::tr("QPaMaT"),
            QObject::tr("An unknown error occurred:\n%1\nThe application will be closed.")
            .arg(e.what()),
            QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
    
    delete qpamat;
    return returncode;
}


