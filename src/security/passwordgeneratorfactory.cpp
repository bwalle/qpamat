/*
 * Id: $Id: passwordgeneratorfactory.cpp,v 1.2 2003/12/17 21:54:19 bwalle Exp $
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
#include <qstring.h>

#include "passwordgenerator.h"
#include "passwordgeneratorfactory.h"
#include "randompasswordgenerator.h"
#include "externalpasswordgenerator.h"

// -------------------------------------------------------------------------------------------------
const PasswordGeneratorFactory::PasswordGeneratorType PasswordGeneratorFactory::DEFAULT_GENERATOR 
    = TRandomPasswordGenerator;
const QString PasswordGeneratorFactory::DEFAULT_GENERATOR_STRING = "RANDOM";
const int     PasswordGeneratorFactory::DEFAULT_LENGTH = 8;

// -------------------------------------------------------------------------------------------------
PasswordGenerator* PasswordGeneratorFactory::getGenerator(PasswordGeneratorType type,
    const QString& additionalArgument) throw (std::invalid_argument)
// -------------------------------------------------------------------------------------------------
{
    switch (type)
    {
        case TRandomPasswordGenerator:
            return new RandomPasswordGenerator();
        case TExternalPasswordGenerator:
            return new ExternalPasswordGenerator(additionalArgument);
        default:
            throw std::invalid_argument("PasswordGeneratorFactory::getChecker: "
                "type is out of range");
    }
}


// -------------------------------------------------------------------------------------------------
PasswordGenerator* PasswordGeneratorFactory::getGenerator(const QString& type, const QString&
    additionalArgument) throw (std::invalid_argument)
// -------------------------------------------------------------------------------------------------
{
    return getGenerator(fromString(type), additionalArgument);
}


// -------------------------------------------------------------------------------------------------
PasswordGeneratorFactory::PasswordGeneratorType PasswordGeneratorFactory::fromString(QString type)
// -------------------------------------------------------------------------------------------------
             throw (std::invalid_argument)
{
    type = type.upper();
    if (type == "RANDOM")
    {
        return TRandomPasswordGenerator;
    }
    else if (type == "EXTERNAL")
    {
        return TExternalPasswordGenerator;
    }
    else
    {
        throw std::invalid_argument(QString("PasswordGeneratorFactory::fromString: Type '%1' "
            "is not valid.").arg(type).latin1());
    }
}


// -------------------------------------------------------------------------------------------------
QString PasswordGeneratorFactory::toString(PasswordGeneratorType type)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument)
{
    switch (type)
    {
        case TRandomPasswordGenerator:
            return "RANDOM";
        case TExternalPasswordGenerator:
            return "EXTERNAL";
        default:
            throw std::invalid_argument("PasswordGeneratorFactory::toString: type is out of range");
    }
}

