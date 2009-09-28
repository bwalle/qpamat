/*
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
#include <cstdlib>
#include <iostream>

#include <QString>
#include <QTranslator>
#include <QTextCodec>
#include <QApplication>
#include <QDir>

#include "qpamat.h"
#include "qpamatwindow.h"
#include "qpamatadaptor.h"
#include "global.h"

/**
 * @class Qpamat
 *
 * @brief This class is the main class.
 *
 * This class represents the application. While the GUI-part of the application is represented by
 * the QpamatWindow, this class represents all the clue stuff which was formerly implemented in the
 * main.cpp file as functions.
 *
 * @ingroup misc
 * @author Bernhard Walle
 */


Qpamat *Qpamat::m_instance = NULL;
QpamatWindow* qpamatwindow;

/**
 * @brief Constructor
 *
 * Creates a new instance of Qpamat.
 */
Qpamat::Qpamat()
    : m_qpamatWindow(new QpamatWindow())
{
    qpamatwindow = m_qpamatWindow.get();
}

/**
 * @brief Installs the tranlators.
 *
 * Installs both the translator for the application and the translator for Qt strings.
 */
void Qpamat::installTranslations()
{
    // translation
    QTranslator translator(0), ttranslator(0);
    QString loc = QTextCodec::locale();
    translator.load(loc, qApp->applicationDirPath() + "/../share/qpamat/translations/");

    QString dirs[] = {
            QString("/usr/share/qt4/translations"),
            QString("/usr/share/qt/translations"),
            QString(getenv("QTDIR")) + "/translations/",
            qApp->applicationDirPath() + "/../share/qpamat/translations/"
    };

    for (unsigned int i = 0; i < ARRAY_SIZE(dirs); i++) {
        QDir test(dirs[i]);
        if (test.exists()) {
            ttranslator.load(QString("qt_") + loc, dirs[i]);
            break;
        }
    }

    qApp->installTranslator(&translator);
    qApp->installTranslator(&ttranslator);
}

/** 
 * @brief Registers the DBus handlers.
 */
void Qpamat::registerDBus()
{
#ifdef Q_WS_X11
	// register the remote interface
	new QpamatAdaptor(qpamatwindow);
	QDBusConnection::sessionBus().registerService("de.berlios.Qpamat");
	QDBusConnection::sessionBus().registerObject("/Qpamat", qpamatwindow);
#endif
}

/**
 * Parses the command line and calls the right functions.
 *
 * Call this function after creating a QApplication.
 *
 * @param[in] argc the number of arguments
 * @param[in] argv an array of strings
 */
void Qpamat::parseCommandLine(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        QString string = QString::fromLatin1(argv[i]);
        if (string == "-h" || string == "--help" || string == "-help") {
            printCommandlineOptions();
            std::exit(0);
        } else if (string == "v" || string == "--version" || string == "-version") {
            printVersion();
            std::exit(0);
        }
    }
}

/**
 * Prints the available command line options on stdout and exits the program.
 */
void Qpamat::printCommandlineOptions()
{
    std::cerr
        << "\n"
        << "This is QPaMaT " << VERSION_STRING << ", a password managing tool for Unix, MacOS X\n"
        << "and Windows using the Qt programming library from Trolltech.\n\n"
        << "Options: -h            prints this help\n"
        << std::endl;
}

/**
 * Prints the version of the program on stderr and exits the program.
 */
void Qpamat::printVersion()
{
    std::cerr << "QPaMaT version " << VERSION_STRING << std::endl;
}

/** 
 * @brief Returns the window
 * 
 * @return the window object
 */
QpamatWindow *Qpamat::getWindow()
{
    return m_qpamatWindow.get();
}

Qpamat *Qpamat::instance()
{
    if (!m_instance) {
        m_instance = new Qpamat();
    }

    return m_instance;
}


// vim: set sw=4 ts=4 et:
