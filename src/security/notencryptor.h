/*
 * Id: $Id: notencryptor.h,v 1.1 2003/12/13 22:34:01 bwalle Exp $
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
#ifndef NOTENCRYPTOR_H
#define NOTENCRYPTOR_H

#include <qstring.h>
#include <qstringlist.h>

#include "../global.h"
#include "stringencryptor.h"

/*!
 * \brief This encryptor does nothing.
 *
 * Yes, it's just a dummy object for writing the plain data into XML.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/13 22:34:01 $
 */
class NotEncryptor : public StringEncryptor
{
    public:
        
        /*!
         * \copydoc StringEncryptor::encryptStrToStr()
         */
        QString encryptStrToStr(const QString& string);
        
        /*!
         * \copydoc StringEncryptor::decryptStrFromStr()
         */
        QString decryptStrFromStr(const QString& string);
        
};

#endif // NOTENCRYPTOR_H
