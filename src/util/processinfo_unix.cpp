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
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "processinfo.h"

/**
 * @class ProcessInfo
 *
 * @brief Information about the current process
 *
 * @ingroup util
 * @author Bernhard Walle
 */

/**
 * @brief Checks if the specified process is currently running.
 *
 * @param pid the PID of the process that should be checked
 * @return \c true if the process is running, \c false otherwise.
 */
bool ProcessInfo::isProcessRunning(q_pid_t pid)
{
    return !kill(pid, 0);
}

/**
 * @brief Returns the PID (process ID) of the process that is currently active.
 *
 * @return the PID of the current proces.
 */
q_pid_t ProcessInfo::getCurrentPid()
{
    return getpid();
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
