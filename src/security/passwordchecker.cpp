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
#include "passwordchecker.h"

// -------------------------------------------------------------------------------------------------
//                                     Static members
// -------------------------------------------------------------------------------------------------

/*!
    Number of checks a "modern" computer can do in one second. From time to time this
    value will be increased. It is irrelevant how fast the computer on which QPaMaT runs
    is, the cracker uses another machine!
*/
const uint PasswordChecker::CRACKS_PER_SECOND = 500000;

/*!
    \class PasswordChecker

    \brief This in an interface for a password checker.

    A password checker checks the quality of a password. The quality is measured in days that a
    cracker needs to crack the password. We assume 500000 tests per second, that is defined in the
    constant CRACKS_PER_SECOND, defined in this class.

    \ingroup security
    \author Bernhard Walle
*/

/*!
    \fn PasswordChecker::~PasswordChecker

    Deletes the SimplePasswordChecker object.
*/

/*!
    \fn PasswordChecker::passwordQuality(const QString&)

    Checks the password.
    \param password the password to check
    \return the number of days that a cracker needs to crack according to the
            password checker
    \exception PasswordCheckException if checking the password failed, e.g. a file does
               not exist
*/

// -------------------------------------------------------------------------------------------------

/*!
    \class PasswordCheckException

    \brief Exception that is thrown if password checking failed.

    There can be different reasons for this. One reason could be that executing an external
    program failed. Another reason could be that a specified file does not exist.

    \ingroup security
    \author Bernhard Walle
*/

/*!
    \fn PasswordCheckException::PasswordCheckException(const std::string&)

    Creates a new instance of the exception and includes the error message. This
    message is returned by the what() method.
    \param error the error message
*/

// :maxLineLen=100:
