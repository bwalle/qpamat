/*
 * Id: $Id: passwordgeneratorfactory.h,v 1.1 2003/12/16 22:50:06 bwalle Exp $
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
#ifndef PASSWORDGENERATORFACTORY_H
#define PASSWORDGENERATORFACTORY_H

#include <stdexcept>

#include <qstring.h>

#include "passwordgenerator.h"

/*!
 * \brief Factory for creating password checkers.
 *
 * This class contains a static method which is used to create instances of a
 * PasswordGenerator. The generators are created with a static method 
 * getChecker(). QPaMaT provides different password generators:
 *
 * - \c RANDOM: RandomPasswordGenerator
 * 
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/16 22:50:06 $
 */
class PasswordGeneratorFactory
{
    public:
        
        /*! Enumeration type for the password checker. */
        enum PasswordGeneratorType 
        { 
            TRandomPasswordGenerator          //!< Represents the RandomPasswordGenerator
        };
        
        /*! The default password checker */
        static const PasswordGeneratorType DEFAULT_GENERATOR;
        
        /*! The default password checker as string. */
        static const QString DEFAULT_GENERATOR_STRING;
        
        /*!
         * Creates a new instance of a PasswordGenerator class. 
         * \param type the type of the password generator
         * \return a pointer to the creates PasswordChecker object. This object must be delected
         *         by the caller.
         */
        static PasswordGenerator* getGenerator(PasswordGeneratorType type) throw ();
            
        /*!
         * This method is provided for convenience. It behaves exactly like the above function.
         * It just calls fromString to get the type reperesentation of the string.
         * \param type the type of the password generator
         * \return a pointer to the created PasswordGenerator object. This object must be delected
         *         by the caller.
         * \exception std::invalid_argument if a wrong \p type is specified
         */
        static PasswordGenerator* getGenerator(const QString& type) throw (std::invalid_argument);
        
        /*!
         * Converts the string representation which is one of \c RANDOM to a PasswordGeneratorType.
         * \param type the string representation
         * \return the enumeration type representation
         * \exception std::invalid_argument if the string reperesentation is invalid
         */
        static PasswordGeneratorType fromString(QString type) throw (std::invalid_argument);
        
        /*!
         * Converts the PasswordGeneratorType to a string representation.
         * \param type the password checker type
         * \return the string representation, one of \c SIMPLE, \c EXTENDED, \c EXTENDEDDICT or 
         *         \c EXTERNAL
         * \exception std::invalid_argument if the type is not a valid PasswordGeneratorType
         */
        static QString toString(PasswordGeneratorType type) throw (std::invalid_argument);
};


#endif // PASSWORDGENERATORFACTORY_H
