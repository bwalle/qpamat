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

#include "ansicolor.h"

/**
 * @class AnsiColor
 *
 * @brief Color codes for ANSI terminals.
 *
 * This class contains constants that represent color codes for ANSI terminals.
 * You can safely assume that any Unix terminal emulation (like Xterm, Konsole,
 * etc.) understands these codes.
 *
 * Of course it would be possible to use Termcap/Terminfo for that. However,
 * there is no practial benefit for that. And even on DOS and Windows consoles,
 * that color code should work.
 *
 * Example usage:
 *
 * @code
 * std::cout << AnsiColor::BRIGHT_RED
 *           << "Something important"
 *           << AnsiColor::NORMAL
 *           << "\n"
 * @endcode
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup misc
 *
 */

/// Resets all color codes.
const char *AnsiColor::NORMAL           = "\033[0m";

/// Enables bold text
const char *AnsiColor::BOLD             = "\033[1m";

/// Black color (not bold)
const char *AnsiColor::BLACK            = "\033[30m";

/// Red color (not bold)
const char *AnsiColor::RED              = "\033[31m";

/// Green color (not bold)
const char *AnsiColor::GREEN            = "\033[32m";

/// Yellow color (not bold)
const char *AnsiColor::YELLOW           = "\033[33m";

/// Blue color (not bold)
const char *AnsiColor::BLUE             = "\033[34m";

/// Magenta color (not bold)
const char *AnsiColor::MAGENTA          = "\033[35m";

/// Cyan color (not bold)
const char *AnsiColor::CYAN             = "\033[36m";

/// White color (not bold)
const char *AnsiColor::WHITE            = "\033[37m";

/// Black color (bold)
const char *AnsiColor::BRIGHT_BLACK     = "\033[30;1m";

/// Red color (bold)
const char *AnsiColor::BRIGHT_RED       = "\033[31;1m";

/// Green color (bold)
const char *AnsiColor::BRIGHT_GREEN     = "\033[32;1m";

/// Yellow color (bold)
const char *AnsiColor::BRIGHT_YELLOW    = "\033[33;1m";

/// Blue color (bold)
const char *AnsiColor::BRIGHT_BLUE      = "\033[34;1m";

/// Magenta color (bold)
const char *AnsiColor::BRIGHT_MAGENTA   = "\033[35;1m";

/// Cyan color (bold)
const char *AnsiColor::BRIGHT_CYAN      = "\033[36;1m";

/// White color (bold)
const char *AnsiColor::BRIGHT_WHITE     = "\033[37;1m";

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
