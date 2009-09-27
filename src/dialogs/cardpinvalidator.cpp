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
#include <QString>

#include "cardpinvalidator.h"
#include "global.h"

/*!
    \class CardPINValidator

    \brief Validates a PIN for a smartcard

    This class acts as a validator for PINs protecting smartcards. Following conditions
    must the PIN met:

     - The PIN must consist of exactly six characters
     - Valid characters are hexadecimal digits (\c 0-9, \c a-f, \c A-F)

    \ingroup dialogs
    \author Bernhard Walle

*/

/*!
    Creates a new instance of a CardPINValidator.
    \param parent the parent widget
    \param name the widget's name
*/
CardPINValidator::CardPINValidator(QObject* parent, const char* name )
    : QValidator(parent, name)
{}


/*!
    Validates the input according to the rule described in the constructor documentation.
    \param input the word to validate
    \param pos no meaning here
    \return Acceptable if the string has excatly 6 hex characters, Invalid if it contains invalid
            characters and Intermediate otherwise
*/
CardPINValidator::State CardPINValidator::validate(QString& input, int& pos) const
{
    UNUSED(pos);

    // invalid characters
    QRegExp hex("^[0-9a-fA-F]*$");
    if (!hex.exactMatch(input))
        return Invalid;

    if (input.length() < 6)
        return Intermediate;
    else if (input.length() == 6)
        return Acceptable;
    else
        return Invalid;
}

// vim: set sw=4 ts=4 et:
