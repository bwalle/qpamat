/*
 * Id: $Id: passwordcheckerfactory.h,v 1.1 2003/12/04 11:56:05 bwalle Exp $
 * ------------------------------------------------------------------------------------------------
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
#ifndef PASSWORDCHECKERFACTORY_H
#define PASSWORDCHECKERFACTORY_H

#include <stdexcept>

#include <qstring.h>

#include "passwordchecker.h"

/*!
 * \brief Factory for creating password checkers.
 *
 * This class contains a static method which is used to create instances of a
 * PasswordChecker. The checkers are created with a static method 
 * getChecker(). QPaMaT provides different password checkers:
 *
 * - \c SIMPLE: SimplePasswordChecker
 * - \c EXTENDED: ExtendedPasswordChecker
 * - \c EXTENDEDDICT: ExtendedDictPasswordChecker
 * - \c EXTERNAL: ExternalPasswordChecker
 * 
 * The additional argument is passed to the constructor. For the ExtendedDictPasswordChecker
 * it's the path of the text file, for ExternalPasswordChecker it's the program name. See the
 * documentation of the classes for details.
 *
 * It is meant to save the additional argument in a configuration file so that the code that
 * calls the getChecker() method does not need to know which object is to created.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/04 11:56:05 $
 */
class PasswordCheckerFactory
{
    public:
        
        /*! Enumeration type for the password checker. */
        enum PasswordCheckerType 
        { 
            TSimplePasswordChecker,           //!< Represents the SimplePasswordChecker
            TExtendedPasswordChecker,         //!< Represents the ExtendedPasswordChecker
            TExtendedDictPasswordChecker,     //!< Extended Password Checker based on a dictionary
            TExternalPasswordChecker          //!< Using an external application for checking
        };
        
        /*! The default password checker */
        static const PasswordCheckerType DEFAULT_CHECKER;
        
        /*! The default password checker as string. */
        static const QString DEFAULT_CHECKER_STRING;
        
        /*!
         * Creates a new instance of a PasswordChecker class. 
         * \param type the type of the password checker
         * \param additionalArgument see class documentation.
         * \return a pointer to the creates PasswordChecker object. This object must be delected
         *         by the caller.
         * \exception std::invalid_argument if the \p additionalArgument is not valid.
         */
        static PasswordChecker* getChecker(PasswordCheckerType type, const QString& 
            additionalArgument = QString::null) throw (std::invalid_argument);
            
        /*!
         * This method is provided for convenience. It behaves exactly like the above function.
         * It just calls fromString to get the type reperesentation of the string.
         * \param type the type of the password checker
         * \param additionalArgument see class documentation.
         * \return a pointer to the creates PasswordChecker object. This object must be delected
         *         by the caller.
         * \exception std::invalid_argument if a wrong \p type is specified or if the
         *             \p additionalArgument is not valid.
         */
        static PasswordChecker* getChecker(const QString& type, const QString& additionalArgument
            = QString::null) throw (std::invalid_argument);
        
        /*!
         * Converts the string representation which is one of \c SIMPLE, \c EXTENDED, 
         * \c EXTENDEDDICT or \c EXTERNAL to a PasswordCheckerType.
         * \param type the string representation
         * \return the enumeration type representation
         * \exception std::invalid_argument if the string reperesentation is invalid
         */
        static PasswordCheckerType fromString(QString type) throw (std::invalid_argument);
        
        /*!
         * Converts the PasswordCheckerType to a string representation.
         * \param type the password checker type
         * \return the string representation, one of \c SIMPLE, \c EXTENDED, \c EXTENDEDDICT or 
         *         \c EXTERNAL
         * \exception std::invalid_argument if the type is not a valid PasswordCheckerType
         */
        static QString toString(PasswordCheckerType type) throw (std::invalid_argument);
};


#endif // PASSWORDCHECKERFACTORY_H
