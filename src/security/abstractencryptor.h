/*
 * Id: $Id: abstractencryptor.h,v 1.1 2003/12/06 18:25:21 bwalle Exp $
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
#include "../types.h"
#include "encryptor.h"

/*!
 * \brief Abstract base class for Encryptor objects.
 *
 * This class implements the Encryptor::encryptString() and Encryptor::decryptString() methods
 * by passing converting data and passing them to the Encryptor::encrypt() or Encryptor::decrypt()
 * methods.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/06 18:25:21 $
 */
class AbstractEncryptor : public Encryptor
{
    public:
        
        /*!
         * \copydoc Encryptor::encrypt()
         */
        virtual ByteVector encrypt(const ByteVector& vector) const = 0;
        
        /*!
         * \copydoc Encryptor::encryptString()
         */
        virtual ByteVector encryptString(const QString& string) const;
        
        /*!
         * \copydoc Encryptor::decrypt()
         */
        virtual ByteVector decrypt(const ByteVector& vector) const = 0;
        
        /*!
         * \copydoc Encryptor::decryptString()
         */
        virtual QString decryptString(const ByteVector& vector) const;
};

#endif // ABSTRACTENCRYPTOR_H
