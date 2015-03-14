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
#include <limits>

#include <QString>

#include "global.h"
#include "masterpasswordchecker.h"


/**
 * @class MasterPasswordChecker
 *
 * @brief This is a simple password checker.
 *
 * A password is valid according to this checker if the following conditions are met:
 *
 *   - the password must have a minimum length of eight characters
 *   - it must contain at least one special character or digit, one uppercase letter
 *     and one lowercase letter
 *
 * @ingroup security
 * @author Bernhard Walle
 */

/**
 * @brief Returns the password quality.
 *
 * We don't calculate how long it takes to crack the password etc. We have a fix policy here.
 *
 * @param password the password to check
 * @return 0.0 if the password is bad, numeric_limits<double>::max() if it is good.
 */
double MasterPasswordChecker::passwordQuality(const QString& password)
{
    if (password.length() < 8) {
        return false;
    }

    bool uppercase = false;
    bool lowercase = false;
    bool nonLetter = false;
    for (int i = 0; i < password.length(); ++i) {
        const QChar character = password[i];
        QChar::Category cat = character.category();
        if (cat == QChar::Letter_Uppercase)
            uppercase = true;
        else if (cat == QChar::Letter_Lowercase)
            lowercase = true;
        else if (! character.isLetter())
            nonLetter = true;
    }

    return uppercase && lowercase && nonLetter ? std::numeric_limits<double>::max() : 0.0;
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
