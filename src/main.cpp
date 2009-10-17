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
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>

#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QTextCodec>

#include "qpamat.h"
#include "qpamatwindow.h"

#include "global.h"
#include "settings.h"
#include "main.h"
#include "util/singleapplication.h"
#include "util/timeoutapplication.h"
#include "qpamatadaptor.h"


int main(int argc, char** argv)
{
    TimeoutApplication app(argc, argv);

    Qpamat *qpamat = Qpamat::instance();
    qpamat->parseCommandLine(argc, argv);

    SingleApplication::init(QDir::homeDirPath(), "QPaMaT");

    try {
        SingleApplication::startup();
        SingleApplication::registerStandardExitHandlers();

        QpamatWindow *win = qpamat->getWindow();
        app.setMainWidget(win);
        qpamat->registerDBus();

        QObject::connect(qpamat->getWindow(), SIGNAL(quit()), &app, SLOT(quit()));
        if (!(win->set().readBoolEntry("Presentation/StartHidden")
              && win->set().readBoolEntry("Presentation/SystemTrayIcon"))) {
            win->show();
        }

        return app.exec();

    } catch (const std::bad_alloc& e) {
        UNUSED(e);
        QMessageBox::warning(0, QObject::tr("QPaMaT"),
            QObject::tr("No more memory available. The application\nwill be closed."),
            QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    } catch (const std::exception& e) {
        QMessageBox::warning(0, QObject::tr("QPaMaT"),
            QObject::tr("An unknown error occurred:\n%1\nThe application will be closed.")
            .arg(e.what()),
            QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
}

// vim: set sw=4 ts=4 et:
