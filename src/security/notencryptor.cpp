/*
 * Id: $Id: notencryptor.cpp,v 1.1 2003/12/13 22:34:01 bwalle Exp $
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
#include <qstring.h>
#include <qstringlist.h>

#include "nosuchalgorithmexception.h"
#include "../global.h"
#include "notencryptor.h"


// -------------------------------------------------------------------------------------------------
QString NotEncryptor::encryptStrToStr(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    return string;
}


// -------------------------------------------------------------------------------------------------
QString NotEncryptor::decryptStrFromStr(const QString& string)
// -------------------------------------------------------------------------------------------------
{
    return string;
}


