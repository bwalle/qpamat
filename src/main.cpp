/*
 * Id: $Id: main.cpp,v 1.9 2003/12/14 16:54:47 bwalle Exp $
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

#ifdef Q_WS_X11
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#endif

#include "qpamat.h"
#include "settings.h"
#include "main.h"

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
    bool nameAvailable = (uname(&uname_struct) == 0);
#endif

    std::cerr 
        << "QPaMaT version   " << VERSION << "\n"
        << "Qt version       " << QT_VERSION_STR << "\n"
#ifdef Q_WS_X11
        << "OS name          " << (nameAvailable ? uname_struct.sysname : "unknown") << "\n"
        << "OS release       " << (nameAvailable ? uname_struct.release : "unknown") << "\n"
        << "OS version       " << (nameAvailable ? uname_struct.version : "unknown") << "\n"
        << "OS machine       " << (nameAvailable ? uname_struct.machine : "unknown")
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
void qpamatNS::singleAppStartup()
// -------------------------------------------------------------------------------------------------
{
    QString lockfile = QDir::homeDirPath() + "/" + ".qpamat.lock";
    QFile file(lockfile);
    
    // check if the file exists
    if (QFile::exists(lockfile))
    {
        QString id = "???";
        if (file.open(IO_ReadOnly))
        {
            QTextStream textstream(&file);
            textstream >> id;
        }
        
        QMessageBox::critical(0, "QPaMaT", QObject::tr("You can only start one instance of "
            "QPaMaT. If you are\nreally sure that no other instance is running, "
            "delete\nthe file %1 and start again."
#ifdef Q_WS_X11
            "\n\n(PID of the currently running process should be %2.)"
#endif
            ).arg(lockfile)
#ifdef Q_WS_X11
            .arg(id)
#endif
            , QMessageBox::Ok, 
            QMessageBox::NoButton);
        std::exit(1);
    }
    
    
    if (!file.open(IO_WriteOnly))
    {
        qDebug("Could not open the file %s for writing.", lockfile.latin1());
        return;
    }
    QTextStream textstream(&file);
#ifdef Q_WS_X11
    textstream << "Process ID only implemented on X11.";
#else
    textstream << getpid();
#endif
}


// -------------------------------------------------------------------------------------------------
void qpamatNS::singleAppEnd()
// -------------------------------------------------------------------------------------------------
{
    QString lockfile = QDir::homeDirPath() + "/" + ".qpamat.lock";
    if (!QFile::remove(lockfile))
    {
        qDebug("Could not remove the lockfile %s", lockfile.latin1());
    }
}



// -------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
// -------------------------------------------------------------------------------------------------
{
    int returncode = 0;
    
    QApplication app(argc, argv);
    qpamatNS::parseCommandLine(argc, argv);
    
    try
    {
        qpamatNS::singleAppStartup();
        
        qpamat = new Qpamat();
        app.setMainWidget(qpamat);
        
        qpamat->show();
        
        returncode = app.exec();
        
        qpamatNS::singleAppEnd();
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


