/*
 * Id: $Id: notencryptor.cpp,v 1.2 2003/12/29 10:59:16 bwalle Exp $
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

#include "global.h"
#include "notencryptor.h"

/*!
    \class NotEncryptor
    
    \brief This encryptor does nothing.
    
    Yes, it's just a dummy object for writing the plain data into XML.
    
    \ingroup security
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date: 2003/12/29 10:59:16 $
*/

/*!
    \copydoc StringEncryptor::encryptStrToStr()
*/
QString NotEncryptor::encryptStrToStr(const QString& string)
{
    return string;
}


/*!
    \copydoc StringEncryptor::decryptStrFromStr()
*/
QString NotEncryptor::decryptStrFromStr(const QString& string)
{
    return string;
}

