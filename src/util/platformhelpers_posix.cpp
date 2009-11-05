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

#include "platformhelpers.h"

/**
 * @class PlatformHelpers
 *
 * @brief Some helper functions which are different across platforms
 *
 * Some static methods that are different across platforms, mainly POSIX and
 * Win32.
 *
 * @ingroup misc
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

/**
 * @enum PlatformHelpers::FileChannel
 * Platform-independent representation of standard input, standard
 * output and standard error.
 */

/**
 * @var PlatformHelpers::FileChannel PlatformHelpers::FC_STDIN
 * Standard input.
 */

/**
 * @var PlatformHelpers::FileChannel PlatformHelpers::FC_STDOUT
 * Standard output.
 */

/**
 * @var PlatformHelpers::FileChannel PlatformHelpers::FC_STDERR
 * Standard error.
 */

/**
 * @brief Checks if @p channel is a terminal
 *
 * Checks if the file channel @p channel (standard input, standard output or
 * standard error) is a terminal. This is an implementation of the isatty()
 * function in POSIX.
 *
 * @param[in] channel the file channel
 * @return @c true if @p channel is a terminal, @c false otherwise.
 *
 * @todo Implement that function on Win32. Currently, the Win32 version always
 *       returns @c false.
 */
bool PlatformHelpers::isTerminal(FileChannel channel)
{
    int fd;

    switch (channel) {
        case FC_STDIN:
            fd = STDIN_FILENO;
            break;

        case FC_STDOUT:
            fd = STDOUT_FILENO;
            break;

        case FC_STDERR:
            fd = STDERR_FILENO;
            break;
    }

    return isatty(fd);
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
