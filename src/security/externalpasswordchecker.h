/*
 * Id: $Id: externalpasswordchecker.h,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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
#ifndef EXTERNALPASSWORDCHECKER_H
#define EXTERNALPASSWORDCHECKER_H

#include <stdexcept>

#include <qstring.h>
#include <qwaitcondition.h>

#include "passwordchecker.h"
#include "passwordcheckexception.h"

/*!
 * \brief This is a password checker that calls an external application. 
 * 
 * It gives the password as string in stdin and determins with the return code of the program 
 * whether the password is Ok. The program may return 0 if the password is Ok and any other 
 * code if not.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/04 11:56:05 $
 */
class ExternalPasswordChecker : public PasswordChecker
{
    public:
        
        /*! The timeout for the application call. */
        static const int TIMEOUT;
        
        /*!
         * Creates a new instance of ExternalPasswordChecker. 
         * \param applicationName the name of the application which should be called to determine
         *        if the password is Ok
         * \exception std::invalid_argument if the application does not exist
         */
        ExternalPasswordChecker(const QString& applicationName) throw (std::invalid_argument);
        
        /*!
         * \copydoc PasswordChecker::isPasswordOk(const QString&)
         */
        bool isPasswordOk(const QString& password) throw (PasswordCheckException);
        
        /*!
         * \copydoc PasswordChecker::minimalLength()
         */
        uint minimalLength();
        
    private:
        QString m_programName;
};

#endif // EXTERNALPASSWORDCHECKER_H
