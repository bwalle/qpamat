/*
 * Id: $Id: extendedpasswordchecker.h,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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
#ifndef EXTENDEDPASSWORDCHECKER_H
#define EXTENDEDPASSWORDCHECKER_H

#include <qstring.h>

#include "passwordchecker.h"
#include "passwordcheckexception.h"

/*!
 * \brief This is a extended password checker for more security. 
 *
 * A password is valid according to this checker if the following conditions are met:
 * 
 *  - the password must have a minimum length of eight characters
 *  - it must contain at least one special character, one digit, one uppercase letter and one
 *    lowercase letter
 *  - each character should occur only once, only one character that occures twice is allowed.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/04 11:56:05 $
 */
class ExtendedPasswordChecker : public PasswordChecker
{
    public:
        
        /*!
         * \copydoc PasswordChecker::isPasswordOk
         */
        bool isPasswordOk(const QString& password) throw (PasswordCheckException);
        
        /*!
         * \copydoc PasswordChecker::minimalLength()
         */
        uint minimalLength();
};

#endif // EXTENDEDPASSWORDCHECKER_H
