/*
 * Id: $Id: abstractencryptor.h,v 1.2 2003/12/10 21:47:39 bwalle Exp $
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
#ifndef ABSTRACTENCRYPTOR_H
#define ABSTRACTENCRYPTOR_H

#include <qstring.h>
#include <qstringlist.h>

#include "nosuchalgorithmexception.h"
#include "../global.h"
#include "encryptor.h"

/*!
 * \brief Abstract base class for Encryptor objects.
 *
 * This class implements the Encryptor::encryptStrToBytes() and Encryptor::decryptStrFromBytes() 
 * methods by converting data and passing them to the Encryptor::encrypt() or Encryptor::decrypt()
 * methods.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/10 21:47:39 $
 */
class AbstractEncryptor : public Encryptor
{
    public:
        
        /*!
         * \copydoc Encryptor::encryptStrToBytes()
         */
        ByteVector encryptStrToBytes(const QString& string);
        
        /*!
         * \copydoc StringEncryptor::encryptStrToStr()
         */
        QString encryptStrToStr(const QString& string);
        
        /*!
         * \copydoc Encryptor::decryptStrFromBytes()
         */
        QString decryptStrFromBytes(const ByteVector& vector);
        
        /*!
         * \copydoc StringEncryptor::decryptStrFromStr()
         */
        QString decryptStrFromStr(const QString& string);
};

#endif // ABSTRACTENCRYPTOR_H
