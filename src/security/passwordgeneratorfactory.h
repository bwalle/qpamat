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
#ifndef PASSWORDGENERATORFACTORY_H
#define PASSWORDGENERATORFACTORY_H

#include <stdexcept>

#include <QString>

#include "passwordgenerator.h"

class PasswordGeneratorFactory
{
    public:

        enum PasswordGeneratorType {
            TRandomPasswordGenerator,
            TExternalPasswordGenerator
        };

        static const PasswordGeneratorType DEFAULT_GENERATOR;
        static const QString DEFAULT_GENERATOR_STRING;
        static const int DEFAULT_LENGTH;

    public:
        static PasswordGenerator* getGenerator(PasswordGeneratorType type, const QString&
            additionalArgument = QString::null) throw (std::invalid_argument);

        static PasswordGenerator* getGenerator(const QString& type, const QString&
            additionalArgument = QString::null) throw (std::invalid_argument);

        static PasswordGeneratorType fromString(QString type) throw (std::invalid_argument);
        static QString toString(PasswordGeneratorType type) throw (std::invalid_argument);
};

#endif // PASSWORDGENERATORFACTORY_H

// vim: set sw=4 ts=4 et:
