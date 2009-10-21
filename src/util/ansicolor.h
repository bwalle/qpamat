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
#ifndef ANSICOLOR_H
#define ANSICOLOR_H

class AnsiColor
{
    public:
        static const char *NORMAL;
        static const char *BOLD;

        static const char *BLACK;
        static const char *RED;
        static const char *GREEN;
        static const char *YELLOW;
        static const char *BLUE;
        static const char *MAGENTA;
        static const char *CYAN;
        static const char *WHITE;

        static const char *BRIGHT_BLACK;
        static const char *BRIGHT_RED;
        static const char *BRIGHT_GREEN;
        static const char *BRIGHT_YELLOW;
        static const char *BRIGHT_BLUE;
        static const char *BRIGHT_MAGENTA;
        static const char *BRIGHT_CYAN;
        static const char *BRIGHT_WHITE;
};

#endif // ANSICOLOR_H

// vim: set sw=4 ts=4 et:
