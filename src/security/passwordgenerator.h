/*
 * Id: $Id: passwordgenerator.h,v 1.3 2003/12/18 22:00:12 bwalle Exp $
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
#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <qstring.h>

#include "../global.h"
#include "passwordgenerateexception.h"

/*!
 * \brief Interface to generate random passwords.
 *
 * Any implementation of this interface generates passwords (normally random passwords).
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.3 $
 * \date $Date: 2003/12/18 22:00:12 $
 */
class PasswordGenerator
{
    public:
        
        /*!
         * Deletes a PasswordGenerator object.
         */
        virtual ~PasswordGenerator() { }
        
        /*!
         * Generates a random password of the specified length. The method may throw any 
         * arbitrary excepting, depending on the implementation. So read the documentation of
         * the implementation crefully.
         * \param length the length of the password
         * \param allowedChars a vector of all allowed characters or a empty list if all characters
         *                     are allowed
         * \return the password
         * \exception PasswordGenerateException if generation failed
         */
        virtual QString getPassword(uint length, QCharVector allowedChars = QCharVector()) 
            throw (PasswordGenerateException) = 0;
        
        /*!
         * Indicates whether the password generator is slow.
         * \return \c true if it is slow, \c false otherwise
         */
        virtual bool isSlow() const = 0;
};

#endif // PASSWORDGENERATOR_H
