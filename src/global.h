/*
 * Id: $Id: global.h,v 1.4 2003/12/29 15:12:26 bwalle Exp $
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

typedef unsigned char byte;

typedef QValueVector<byte>      ByteVector;
typedef QValueVector<QString>   StringVector;
typedef QValueVector<uint>      UIntVector;
typedef QValueVector<int>       IntVector;

typedef QMap<QString, QString>  StringMap;
typedef QValueVector<QChar>     QCharVector;


