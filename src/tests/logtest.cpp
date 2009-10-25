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

int main(int argc, char *argv[])
{
    qInstallMsgHandler(QpamatDebug::msgHandler);
    QpamatDebug *qpDebug = QpamatDebug::instance();

    std::cerr << "With debug" << std::endl;
    qpDebug->setMessageLevel(QtDebugMsg);
    qpDebug(DEFAULT_COMPONENT) << "Bla";
    qpDebug("Component") << "Bla";
    qpWarning(DEFAULT_COMPONENT) << "Warning";
    qpWarning("Component") << "Warning";
    qpCritical(DEFAULT_COMPONENT) << "Fatal";
    qpCritical("Component") << "Fatal";

    std::cerr << "Without debug" << std::endl;
    qpDebug->setMessageLevel(QtWarningMsg);
    qpDebug(DEFAULT_COMPONENT) << "Bla";
    qpDebug("Component") << "Bla";
    qpWarning(DEFAULT_COMPONENT) << "Warning";
    qpWarning("Component") << "Warning";
    qpCritical(DEFAULT_COMPONENT) << "Fatal";
    qpCritical("Component") << "Fatal";

    std::cerr << "File 'bla.out'" << std::endl;

    qpDebug->setMessageLevel(QtDebugMsg);
    qpDebug->redirectFile("bla.out");
    qpDebug(DEFAULT_COMPONENT) << "Bla";
    qpDebug("Component") << "Bla";
    qpWarning(DEFAULT_COMPONENT) << "Warning";
    qpWarning("Component") << "Warning";
    qpCritical(DEFAULT_COMPONENT) << "Fatal";
    qpCritical("Component") << "Fatal";

    std::cerr << "Console, Filter 'Component'" << std::endl;

    qpDebug->setMessageLevel(QtDebugMsg);
    qpDebug->redirectConsole();
    QStringList l;
    l << DEFAULT_COMPONENT;
    qpDebug->setFilterComponents(l);
    qpDebug(DEFAULT_COMPONENT) << "Bla";
    qpDebug("Component") << "Bla";
    qpWarning(DEFAULT_COMPONENT) << "Warning";
    qpWarning("Component") << "Warning";
    qpCritical(DEFAULT_COMPONENT) << "Fatal";
    qpCritical("Component") << "Fatal";

    return EXIT_SUCCESS;
}

// vim: set sw=4 ts=4 et:
