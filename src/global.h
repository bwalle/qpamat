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
#ifndef GLOBAL_H
#define GLOBAL_H

#include <Q3ValueVector>
#include <QMap>
#include <QString>

/**
 * @file global.h
 * @ingroup gui
 *
 * @brief defines some types and functions used in the whole application.
 *
 * defines some types and functions used in the whole application.
 */


/**
 * @typedef QValueVector<unsigned char> ByteVector
 *
 * This is a unsigned char vector.
 */
typedef Q3ValueVector<unsigned char> ByteVector;

/**
 * @typedef QValueVector<QString> StringVector
 *
 * This is a vector of QString objects.
 */
typedef Q3ValueVector<QString>   StringVector;

/**
 * @typedef QValueVector<unsigned int> UIntVector
 *
 * Vector for unsigned integer.
 */
typedef Q3ValueVector<unsigned int>      UIntVector;

/**
 * @typedef QValueVector<int> IntVector
 *
 * Vector for integer.
 */
typedef Q3ValueVector<int>       IntVector;

/**
 * @typedef QMap<QString, QString> StringMap
 *
 * This is a map that maps strings to strings
 */
typedef QMap<QString, QString>  StringMap;

/**
 * @typedef QValueVector<QChar> QCharVector
 *
 * Vector for Unicode characters.
 */
typedef Q3ValueVector<QChar>     QCharVector;

/**
 * Returns the size of a global array.
 */
#define ARRAY_SIZE(array)       \
    ( sizeof((array)) / sizeof((array)[0]) )

/**
 * Unused parameters.
 *
 * @param param the unused parameter
 */
#define UNUSED(param) (void)param;

#ifndef DOXYGEN
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))
# define _CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define _CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define _CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define _CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define _CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define _CURRENT_FUNCTION __func__
#else
# define _CURRENT_FUNCTION "(unknown)"
#endif
#endif // DOXYGEN

/**
 * Macro for the current function with signature included in brackets. Example:
 *
 * @code
 * [ void Qpamat::setLogin(bool) ]
 * @endcode
 *
 * Since the streaming operators are used, that only works for straming objects. Examples:
 *
 * @code
 * std::cout << CURRENT_FUNCTION << "Hello!" << std::endl;
 * qDebug() << CURRENT_FUNCTION << "Hello!";
 * @endcode
 */
#define CURRENT_FUNCTION \
        "[" << _CURRENT_FUNCTION << "]"

#endif // GLOBAL_H

// vim: set sw=4 ts=4 et:
