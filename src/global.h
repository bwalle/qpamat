/*
 * Id: $Id: global.h,v 1.5 2004/01/02 12:19:40 bwalle Exp $
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


// Copyright (c) 2003 Benedikt Meurer (benedikt.meurer@unix-ag.uni-siegen.de)
// xfwm4: debug.h


#if !defined(DOXYGEN) && (defined(DEBUG) || defined(TRACE))

#if defined(__NetBSD__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __DBG_FUNC__    __func__
#elif defined(__GNUC__) && __GNUC__ >= 3
#define __DBG_FUNC__	__FUNCTION__
#else
#define __DBG_FUNC__    "??"
#endif

#endif // !defined(DOXYGEN) && (defined(DEBUG) || defined(TRACE))


#ifdef DEBUG

#define PRINT_DBG(fmt, args...)                                                  \
{                                                                                \
    qDebug("DEBUG[%s:%d] %s(): \n   "fmt, __FILE__, __LINE__, __DBG_FUNC__, ##args);  \
}

#else

#define PRINT_DBG(fmt, args...)   { do {} while(0); }

#endif


#ifdef TRACE

#define PRINT_TRACE(fmt, args...)                                                 \
{                                                                                 \
    qDebug("TRACE[%s:%d] %s(): \n   "fmt, __FILE__, __LINE__, __DBG_FUNC__, ##args);   \
}

#else

#define PRINT_TRACE(fmt, args...)   { do {} while(0); }

#endif // TRACE

