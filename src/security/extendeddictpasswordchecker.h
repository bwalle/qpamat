/*
 * Id: $Id: extendeddictpasswordchecker.h,v 1.2 2003/12/17 21:53:41 bwalle Exp $
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
#ifndef EXTENDEDDICTPASSWORDCHECKER_H
#define EXTENDEDDICTPASSWORDCHECKER_H

#include <stdexcept>

#include <qstring.h>

#include "extendedpasswordchecker.h"
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
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/17 21:53:41 $
 */
class ExtendedDictPasswordChecker : public ExtendedPasswordChecker
{
    public:
        
        /*!
         * Creates a new instance of a ExtendedDictionaryPasswordChecker object.
         * \param file the file where the dictionary is stored
         * @exception std::invalid_argument if the file is not QString::null and not exist
         */
        ExtendedDictPasswordChecker(const QString& file) throw (std::invalid_argument);
        
        /*!
         * This function checks whether the password is valid.
         * \param password the password to check
         * \return \c true if the password is Ok, \c false if not
         */
        bool isPasswordOk(const QString& password) throw (PasswordCheckException);
        
        /*!
         * Returns true.
         * \return true
         */
        virtual bool isSlow() const;
        
    private:
        QString m_file;
};

#endif // EXTENDEDDICTPASSWORDCHECKER_H
