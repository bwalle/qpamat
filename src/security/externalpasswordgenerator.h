/*
 * Id: $Id: externalpasswordgenerator.h,v 1.1 2003/12/17 21:55:19 bwalle Exp $
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
#ifndef EXTERNALPASSWORDGENERATOR_H
#define EXTERNALPASSWORDGENERATOR_H

#include <qstring.h>

#include "passwordgenerator.h"
#include "passwordgenerateexception.h"

/*!
 * \brief Object to generate random passwords. 
 *
 * Generates passwords using a random application. The application gets the required length
 * as first argument. A shellscript wrapper may be used.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/17 21:55:19 $
 */
class ExternalPasswordGenerator : public PasswordGenerator
{
    public:
        
         /*! The timeout for the application call. */
        static const int TIMEOUT;
        
        /*!
         * Creates a new instance of ExternalPasswordGenerator. 
         * \param applicationName the name of the application which should be called to get
         *        the password
         */
        ExternalPasswordGenerator(const QString& applicationName);
        
        /*!
         * Generates a random password.
         * \param length the length of the password
         * \return the password
         * \exception PasswordGenerateException if launching the external application failed
         */
        QString getPassword(int length) throw (PasswordGenerateException);
        
        /*!
         * Returns \c true
         * \return \c true
         */
        bool isSlow() const;
        
    private:
        const QString m_applicationName;
};

#endif // EXTERNALPASSWORDGENERATOR_H
