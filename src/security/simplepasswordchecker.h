/*
 * Id: $Id: simplepasswordchecker.h,v 1.2 2003/12/17 21:54:52 bwalle Exp $
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
#ifndef SIMPLEPASSWORDCHECKER_H
#define SIMPLEPASSWORDCHECKER_H

#include <qstring.h>

#include "passwordchecker.h"
#include "passwordcheckexception.h"

/*!
 * \brief This is a simple password checker.
 *
 * A password is valid according to this checker if the following conditions are met:
 * 
 *  - the password must have a minimum length of six characters
 *  - it must contain at least one special character or digit, one uppercase letter and one
 *    lowercase letter
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/17 21:54:52 $
 */
class SimplePasswordChecker : public PasswordChecker
{
    public:
        
        /*!
         * \copydoc PasswordChecker::isPasswordOk()
         */
        bool isPasswordOk(const QString& password) throw (PasswordCheckException);
        
        /*!
         * \copydoc PasswordChecker::minimalLength()
         */
        uint minimalLength() const;
        
        /*!
         * Returns false.
         * \return false
         */
        virtual bool isSlow() const;
};

#endif // SIMPLEPASSWORDCHECKER_H
