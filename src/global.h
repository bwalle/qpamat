/*
 * Id: $Id: global.h,v 1.3 2003/12/20 15:58:02 bwalle Exp $
 * -------------------------------------------------------------------------------------------------
 * 
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
#include <qvaluevector.h>
#include <qmap.h>
#include <qstring.h>

/*!
 * \file global.h
 * \ingroup gui
 * Defines some types and functions used in the whole application.
 */


/*!
 * We use unsigned char in meaning of a byte.
 */
typedef unsigned char byte;

/*!
 * This is a byte vector.
 */
typedef QValueVector<byte> ByteVector;

/*!
 * This is a vector of QString objects.
 */
typedef QValueVector<QString> StringVector;

/*!
 * This is a Map which has keys and values of the type QString.
 */
typedef QMap<QString, QString> StringMap;

/*!
 * Vector for unsigned integer.
 */
typedef QValueVector<uint> UIntVector;

/*!
 * Vector for integer.
 */
typedef QValueVector<int> IntVector;

/*!
 * Vector for Unicode characters.
 */
typedef QValueVector<QChar> QCharVector;

// Forward declaration
class Qpamat;

