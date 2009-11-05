/*
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
#include <qstring.h>

#include "passwordgenerator.h"
#include "passwordgeneratorfactory.h"
#include "randompasswordgenerator.h"
#include "externalpasswordgenerator.h"

// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

/**
 * The default password checker
 */
const PasswordGeneratorFactory::PasswordGeneratorType PasswordGeneratorFactory::DEFAULT_GENERATOR
    = TRandomPasswordGenerator;

/**
 * The default password checker as string.
 */
const QString PasswordGeneratorFactory::DEFAULT_GENERATOR_STRING = "RANDOM";

/**
 * Default length of a password.
 */
const int     PasswordGeneratorFactory::DEFAULT_LENGTH = 8;


/**
 * @class PasswordGeneratorFactory
 *
 * @brief Factory for creating password checkers.
 *
 * This class contains a static method which is used to create instances of a
 * PasswordGenerator. The generators are created with a static method getChecker(). QPaMaT
 * provides different password generators:
 *
 *   - @c RANDOM: RandomPasswordGenerator
 *   - @c EXTERNAL: ExternalPasswordGenerator
 *
 * The additional argument is passed to the constructor. For the ExternalPasswordGenerator
 * it's the program name. See the documentation of the classes for details.
 *
 * @ingroup security
 * @author Bernhard Walle
 */

/**
 * @enum PasswordGeneratorFactory::PasswordGeneratorType
 *
 * @brief Enumeration type for the password checker.
 */

/**
 * @var PasswordGeneratorFactory::TRandomPasswordGenerator
 *
 * @brief Represents the RandomPasswordGenerator
 */

/**
 * @var PasswordGeneratorFactory::TExternalPasswordGenerator
 *
 * @brief Represents the ExternalPasswordGenerator
 */

/**
 * @brief Creates a new instance of a PasswordGenerator class.
 *
 * @param type the type of the password generator
 * @param additionalArgument see class documentation.
 *
 * @return a pointer to the creates PasswordChecker object. This object must
 *         be delected by the caller.
 *
 * @exception std::invalid_argument if the additional argument is invalid
 */
PasswordGenerator* PasswordGeneratorFactory::getGenerator(PasswordGeneratorType type,
    const QString& additionalArgument) throw (std::invalid_argument)
{
    switch (type) {
        case TRandomPasswordGenerator:
            return new RandomPasswordGenerator();

        case TExternalPasswordGenerator:
            return new ExternalPasswordGenerator(additionalArgument);

        default:
            throw std::invalid_argument("PasswordGeneratorFactory::getChecker: "
                "type is out of range");
    }
}


/**
 * @brief This method is provided for convenience.
 *
 * It behaves exactly like the above function. It just calls fromString to get the type
 * reperesentation of the string.
 *
 * @param type               the type of the password generator
 * @param additionalArgument see class documentation.
 * @return a pointer to the created PasswordGenerator object. This object must be
 *         delected by the caller.
 * @exception std::invalid_argument if a wrong \p type is specified or if the argument
 *                                  is invalid
 */
PasswordGenerator* PasswordGeneratorFactory::getGenerator(const QString    &type,
                                                          const QString    &additionalArgument)
    throw (std::invalid_argument)
{
    return getGenerator(fromString(type), additionalArgument);
}


/**
 * @brief Converts the string representation which is one of \c RANDOM to a PasswordGeneratorType.
 *
 * @param type the string representation
 * @return the enumeration type representation
 * @exception std::invalid_argument if the string reperesentation is invalid
 */
PasswordGeneratorFactory::PasswordGeneratorType PasswordGeneratorFactory::fromString(QString type)
             throw (std::invalid_argument)
{
    type = type.upper();
    if (type == "RANDOM")
        return TRandomPasswordGenerator;
    else if (type == "EXTERNAL")
        return TExternalPasswordGenerator;
    else
        throw std::invalid_argument(QString("PasswordGeneratorFactory::fromString: Type '%1' "
            "is not valid.").arg(type).latin1());
}


/**
 * @brief Converts the PasswordGeneratorType to a string representation.
 *
 * @param type the password checker type
 * @return the string representation, one of \c SIMPLE, \c EXTENDED or \c EXTERNAL
 * @exception std::invalid_argument if the type is not a valid PasswordGeneratorType
 */
QString PasswordGeneratorFactory::toString(PasswordGeneratorType type)
            throw (std::invalid_argument)
{
    switch (type) {
        case TRandomPasswordGenerator:
            return "RANDOM";

        case TExternalPasswordGenerator:
            return "EXTERNAL";

        default:
            throw std::invalid_argument("PasswordGeneratorFactory::toString: type is out of range");
    }
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
