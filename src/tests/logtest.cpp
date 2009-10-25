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

#include <util/debug.h>

#include "logtest.h"

void TestLog::logConsole() const
{
}

int main(int argc, char *argv[])
{
    qInstallMsgHandler(QpamatDebug::msgHandler);

    std::cerr << "With debug" << std::endl;
    QpamatDebug::instance()->setMessageLevel(QtDebugMsg);
    qDebug("Bla");
    qDebug("Component\tBla");
    qWarning("Warning");
    qWarning("Component\tWarning");
    qCritical("Fatal");
    qCritical("Component\tFatal");

    std::cerr << "Without debug" << std::endl;
    QpamatDebug::instance()->setMessageLevel(QtWarningMsg);
    qDebug("Bla");
    qDebug("Component\tBla");
    qWarning("Warning");
    qWarning("Component\tWarning");
    qCritical("Fatal");
    qCritical("Component\tFatal");

    std::cerr << "File 'bla.out'" << std::endl;

    QpamatDebug::instance()->setMessageLevel(QtDebugMsg);
    QpamatDebug::instance()->redirectFile("bla.out");
    qDebug("Bla");
    qDebug("Component\tBla");
    qWarning("Warning");
    qWarning("Component\tWarning");
    qCritical("Fatal");
    qCritical("Component\tFatal");

    std::cerr << "Console, Filter 'Component'" << std::endl;

    QpamatDebug::instance()->setMessageLevel(QtDebugMsg);
    QpamatDebug::instance()->redirectConsole();
    QStringList l;
    l << QpamatDebug::DEFAULT_COMPONENT;
    QpamatDebug::instance()->setFilterComponents(l);
    qDebug("Bla");
    qDebug("Component\tBla");
    qWarning("Warning");
    qWarning("Component\tWarning");
    qCritical("Fatal");
    qCritical("Component\tFatal");

    return EXIT_SUCCESS;
}

// vim: set sw=4 ts=4 et:
