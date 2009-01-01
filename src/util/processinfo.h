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
#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QObject>

#ifdef Q_WS_WIN
#   include <Windows.h>
#endif

#ifdef Q_WS_WIN
typedef DWORD q_pid_t;
#else
typedef pid_t q_pid_t;
#endif

class ProcessInfo
{
    public:
        static bool isProcessRunning(q_pid_t pid);
        static q_pid_t getCurrentPid();
};

#endif // PROCESSINFO_H

// vim: set sw=4 ts=4 et: :maxLineLen=100:
