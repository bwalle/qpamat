/*
 * Id: $Id: configpasswordchecker.h,v 1.1 2003/12/18 21:57:53 bwalle Exp $
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
#ifndef CONFIGPASSWORDCHECKER_H
#define CONFIGPASSWORDCHECKER_H

#include <qstring.h>
#include <qvaluevector.h>

#include "../global.h"
#include "passwordchecker.h"
#include "passwordcheckexception.h"

/*!
 * \brief This is a configurable password checker.
 *
 * This checker is used for checking the generated password in this application. Following
 * aspects of the generator are configurable:
 * 
 *  - the minimum length a password must have,
 *  - all allowed characters in form of a string. Why do we limit the allowed characters? Because
 *    normally we need characters that are available on \b all systems. For example old Unix
 *    passwords must not contain umlauts and it isn't a very good idea to choose umlauts. What
 *    is if you come to a Workstation with an English keyboard ...?
 *  - ensured character classes: good passwords should contain uppercase letters, lowercase letters,
 *    digits and/or special characters like the dollar sign. Here we provide a string like
 *    \c ULDs. Each letter stands for a group of characters -- \c U stands for uppercase letters,
 *    \c L for lowercase chars, \c D for digits and \c S for special characters. If the letter
 *    is uppercase, the password must contain it, if it is lowercase (or missing), it is equal.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/18 21:57:53 $
 */
class ConfigPasswordChecker : public PasswordChecker
{
    public:
        
        /*!
         * Creates a new instance of a ConfigPasswordChecker.
         * \param length the minimum length a password must have
         * \param ensuredChars see class documentation
         * \param allowedChars a list of allowed chars, <tt>A-Z</tt>, <tt>a-z</tt> and <tt>0-9</tt>
         *        are allowed
         */
        ConfigPasswordChecker(int length, const QString& ensuredChars, const QString& allowedChars);
        
        /*!
         * \copydoc PasswordChecker::isPasswordOk
         */
        bool isPasswordOk(const QString& password) throw (PasswordCheckException);
        
        /*!
         * \copydoc PasswordChecker::minimalLength()
         */
        uint minimalLength() const;
        
        /*!
         * \copydoc PasswordChecker::allowedCharacters()
         */
        QCharVector allowedCharacters() const;
        
    private:
        QCharVector m_allowedChars;
        const QString m_ensuredChars;
        const int m_length;
};

#endif // CONFIGPASSWORDCHECKER_H
