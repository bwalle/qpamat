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
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <memory>

#include <qapplication.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qdesktopwidget.h>
#include <qsettings.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextcodec.h>

#include "qpamat.h"
#include "global.h"
#include "settings.h"
#include "main.h"
#include "util/singleapplication.h"
#include "util/docktimeoutapplication.h"

#ifdef Q_WS_X11
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
namespace X11 
{
    #include <X11/Xlib.h>
};
#endif


Qpamat* qpamat;

/*!
    Prints the available command line options on stdout and exits the program.
*/
void printCommandlineOptions()
{
    std::cerr 
        << "\n" 
        << "This is QPaMaT " << VERSION_STRING << ", a password managing tool for Unix, MacOS X\n"
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


/*!
    Prints the version of the program on stderr and exits the program.
*/
void printVersion()
{
#ifdef Q_WS_X11
    struct utsname uname_struct;
    bool nameAvailable = (uname(&uname_struct) >= 0);
    QString X11protocolVersion, X11vendorVersion;
    getX11Version(X11protocolVersion, X11vendorVersion);
#endif

    std::cerr 
        << "QPaMaT version   " << VERSION_STRING << "\n"
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


/*!
    Parses the command line and calls the right functions. Call this function after
    creating a QApplication.
    \param argc the number of arguments
    \param argv an array of strings
*/
void parseCommandLine(int& argc, char**& argv)
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


/*!
    Returns the X11 version if the code is compiled on a system with X11. If not,
    the strings are not changed.
    \param protocolVersion the version of the X11 protocol, normally 11
    \param vendorVersion the version of the implementation e.g. of XFree86 on Linux
*/
void getX11Version(QString& protocolVersion, QString& vendorVersion)
{
#ifdef Q_WS_X11
    using X11::_XPrivDisplay;
    X11::Display* dpy = X11::XOpenDisplay(0);
    if (dpy)
    {
        protocolVersion = QString("%1.%2").arg(QString::number(ProtocolVersion (dpy))).
            arg(ProtocolRevision (dpy));
        vendorVersion = QString::number(VendorRelease (dpy));
        X11::XCloseDisplay(dpy);
    }
#endif
}

int main(int argc, char** argv)
{
    DockTimeoutApplication app(argc, argv);
    parseCommandLine(argc, argv);
    
    // translation
    QTranslator translator(0), ttranslator(0);
    QString loc = QTextCodec::locale();
    translator.load(loc, qApp->applicationDirPath() + "/../share/qpamat/translations/");
    ttranslator.load(QString("qt_") + loc, QString(getenv("QTDIR")) + "/translations/");
    app.installTranslator(&translator);
    app.installTranslator(&ttranslator);
    
    SingleApplication::init(QDir::homeDirPath(), "QPaMaT");
    std::auto_ptr<Qpamat> qp;
    
    try
    {
        SingleApplication::startup();
        SingleApplication::registerStandardExitHandlers();
        
        qp = std::auto_ptr<Qpamat>(new Qpamat());
        qpamat = qp.get();
        app.setMainWidget(qpamat);
        
        QObject::connect(qpamat, SIGNAL(quit()), &app, SLOT(quit()));
        if (!(qpamat->set().readBoolEntry("Presentation/StartHidden")
                        && qpamat->set().readBoolEntry("Presentation/StartMinimized")))

        {
            qpamat->show();
        }
        
        return app.exec();
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
}

