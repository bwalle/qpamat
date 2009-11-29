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
#ifndef PLATFORMHELPERS_H
#define PLATFORMHELPERS_H

/**
 * @file platformhelpers.h
 * @ingroup misc
 * @brief Contains functions that help for development across multiple platforms
 *
 * That file contains classes that don't fit into other implementation files that contain
 * some platform abstraction. It also contains macros that make case determinations easier.
 *
 * There are following macros:
 *
 *  - @c RUNNING_ON_X11 which is 1 if @c Q_WS_X11 is defined and 0 otherwise
 *  - @c RUNNING_ON_MAC which is 1 if @c Q_WS_MAC is defined and 0 otherwise
 *  - @c RUNNING_ON_POSIX which is 1 if either @c Q_WS_X11 or @c Q_WS_MAC is defined and 0 otherwise
 *  - @c RUNNING_ON_WINDOWS which is 1 if @c Q_WS_WIN is defined and 0 otherwise
 *  - @c RUNNING_ON_EMBEDDED_LINUX which is 1 if @c Q_WS_QWS is defined and 0 otherwise
 *
 * So, instead of writing
 *
 * @code
 * #ifdef Q_WS_X11
 *    do_something();
 * #endif
 * @endcode
 *
 * you should write
 *
 * @code
 * if (RUNNING_ON_X11)
 *    do_something();
 * @endcode
 *
 * which makes the code much more readable.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#ifndef DOXYGEN

#ifdef Q_WS_X11
#  define RUNNING_ON_X11 1
#else
#  define RUNNING_ON_X11 0
#endif

#ifdef Q_WS_MAC
#  define RUNNING_ON_MAC 1
#else
#  define RUNNING_ON_MAC 0
#endif

#if defined(Q_WS_MAC) || defined(Q_WS_X11)
#  define RUNNING_ON_POSIX 1
#else
#  define RUNNING_ON_POSIX 0
#endif

#ifdef Q_WS_WIN
#  define RUNNING_ON_WINDOWS 1
#else
#  define RUNNING_ON_WINDOWS 0
#endif

#ifdef Q_WS_QWS
#  define RUNNING_ON_EMBEDDED_LINUX 1
#else
#  define RUNNING_ON_EMBEDDED_LINUX 0
#endif

#endif // DOXYGEN

class PlatformHelpers
{
    public:
        enum FileChannel {
            FC_STDIN,
            FC_STDOUT,
            FC_STDERR
        };

        static bool isTerminal(FileChannel channel);
};

#endif /* PLATFORMHELPERS_H */

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
