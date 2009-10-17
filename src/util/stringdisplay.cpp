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
#include <QObject>
#include <QLocale>

#include "stringdisplay.h"


/**
 * @class StringDisplay
 *
 * @brief This class provides functions for displaying a amount as pretty string.
 *
 * The only available function at the moment is meant for displaying a amount of time in a
 * suitable unit.
 *
 * @ingroup misc
 * @author Bernhard Walle
 */

/**
 * @brief This function converts a day into a suitable unit
 *
 * This function converts a day into a suitable unit, i.e. 120 days are displayed as 4
 * month.
 *
 * @param days the time in days
 */
QString StringDisplay::displayTimeSuitable(double days)
{
    if (days > 365) {
        QLocale loc;

        int num = int(days / 365);

        if (num <= 1) {
            return QObject::tr("%1 year").arg(loc.toString(num));
        } else {
            return QObject::tr("%1 years").arg(loc.toString(num));
        }
    } else if (days > 30) {
        int num = int(days / 30);
        if (num <= 1) {
            return QObject::tr("%1 month").arg(QString::number(num));
        } else {
            return QObject::tr("%1 months").arg(QString::number(num));
        }
    } else if (days > 7) {
        int num = int(days / 7);
        if (num <= 1) {
            return QObject::tr("%1 week").arg(QString::number(num));
        } else {
            return QObject::tr("%1 weeks").arg(QString::number(num));
        }
    } else if (days >= 1.0) {
        int num = int(days);
        if (num <= 1) {
            return QObject::tr("%1 day").arg(QString::number(num));
        } else {
            return QObject::tr("%1 days").arg(QString::number(num));
        }
    } else {
        int num = int(days * 24);
        if (num <= 1) {
            return QObject::tr("a few minutes");
        } else {
            return QObject::tr("%1 hours").arg(QString::number(num));
        }
    }
}

// vim: set sw=4 ts=4 et:
