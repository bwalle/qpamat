/*
 * Id: $Id: passwordcheckerfactory.cpp,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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

#include "passwordchecker.h"
#include "passwordcheckerfactory.h"
#include "simplepasswordchecker.h"
#include "extendedpasswordchecker.h"
#include "extendeddictpasswordchecker.h"
#include "externalpasswordchecker.h"

// -------------------------------------------------------------------------------------------------
const PasswordCheckerFactory::PasswordCheckerType PasswordCheckerFactory::DEFAULT_CHECKER 
    = TSimplePasswordChecker;
const QString PasswordCheckerFactory::DEFAULT_CHECKER_STRING = "SIMPLE";

// -------------------------------------------------------------------------------------------------
PasswordChecker* PasswordCheckerFactory::getChecker(PasswordCheckerType type,
            const QString& additionalArgument)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument)
{
    switch (type)
    {
        case TSimplePasswordChecker:
            return new SimplePasswordChecker();
        case TExtendedPasswordChecker:
            return new ExtendedPasswordChecker();
        case TExtendedDictPasswordChecker:
            return new ExtendedDictPasswordChecker(additionalArgument);
        case TExternalPasswordChecker:
            return new ExternalPasswordChecker(additionalArgument);
        default:
            throw std::invalid_argument("PasswordCheckerFactory::getChecker: type is out of range");
    }
}


// -------------------------------------------------------------------------------------------------
PasswordChecker* PasswordCheckerFactory::getChecker(const QString& type,
    const QString& additionalArgument)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument)
{
    return getChecker(fromString(type), additionalArgument);
}


// -------------------------------------------------------------------------------------------------
PasswordCheckerFactory::PasswordCheckerType PasswordCheckerFactory::fromString(QString type)
// -------------------------------------------------------------------------------------------------
             throw (std::invalid_argument)
{
    type = type.upper();
    if (type == "SIMPLE")
    {
        return TSimplePasswordChecker;
    }
    else if (type == "EXTENDED")
    {
        return TExtendedPasswordChecker;
    }
    else if (type == "EXTENDEDDICT")
    {
        return TExtendedDictPasswordChecker;
    }
    else if (type == "EXTERNAL")
    {
        return TExternalPasswordChecker;
    }
    else
    {
        throw std::invalid_argument(QString("PasswordCheckerFactory::fromString: Type '%1' "
            "is not valid.").arg(type).latin1());
    }
}


// -------------------------------------------------------------------------------------------------
QString PasswordCheckerFactory::toString(PasswordCheckerType type)
// -------------------------------------------------------------------------------------------------
            throw (std::invalid_argument)
{
    switch (type)
    {
        case TSimplePasswordChecker:
            return "SIMPLE";
        case TExtendedPasswordChecker:
            return "EXTENDED";
        case TExtendedDictPasswordChecker:
            return "EXTENDEDDICT";
        case TExternalPasswordChecker:
            return "EXTERNAL";
        default:
            throw std::invalid_argument("PasswordCheckerFactory::toString: type is out of range");
    }
}

