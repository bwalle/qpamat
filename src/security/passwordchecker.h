/*
 * Id: $Id: passwordchecker.h,v 1.2 2003/12/17 21:54:04 bwalle Exp $
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
#ifndef PASSWORDCHECKER_H
#define PASSWORDCHECKER_H

#include <qstring.h>

#include "passwordcheckexception.h"

/*!
 * \brief This in an interface for a password checker.
 * 
 * A password checker checks if a password is valid.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/17 21:54:04 $
 */
class PasswordChecker
{
    public:
        
        /*!
         * Deletes the SimplePasswordChecker object.
         */
        virtual ~PasswordChecker() { };
        
        /*!
         * This function checks whether the password is valid.
         * \param password the password to check
         * \return \c true if the password is Ok, \c false if not
         * \exception PasswordCheckException if checking the password failed, e.g. a file does
         *            not exist
         */
        virtual bool isPasswordOk(const QString& password) throw (PasswordCheckException) = 0;
        
        /*!
         * Returns the minimal length for a valid password. A value of zero indicates that
         * there's no limitation for the length.
         * \return the number of characters 
         */
        virtual uint minimalLength() const = 0;
        
        /*!
         * Returns if the password checker is slow. Of course, the word slow is relative. Calling
         * an external program is always slow, checking only the length and a few characters
         * is fast.
         * \return \c true if it is slow, \c false otherwise
         */
        virtual bool isSlow() const = 0;
};

#endif // PASSWORDCHECKER_H
