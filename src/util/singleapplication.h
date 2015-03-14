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
#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <stdexcept>

#include <QObject>
#include <QString>

class SingleApplication : public QObject
{
    Q_OBJECT

    public:
        static void init(const QString& lockfileDir, const QString& appName);

        static void registerStandardExitHandlers();

        static void startup();

        static void shutdown();
        static void shutdown(int signal);


    private:
        static QString         lockfile;
        static QString         appName;
        static bool            didShutdownAlready;
        static bool            initialized;
};

#endif // SINGLEAPPLICATION_H

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
