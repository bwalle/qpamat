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
#include <stdexcept>
#include <string>
#include <cstring>

#include "cardexception.h"

/*!
    \class CardException

    \brief Exception for communicating with smartcard.

    This exception is thrown if an error occurred while communicating with the smartcard
    terminal. It just encapsulates the error codes of CT-API in a programmer-friendly way.
    You don't have to deal with strange constants and returncodes.

    \ingroup smartcard
    \author Bernhard Walle
*/

/*!
    \enum CardException::ErrorCode

    Enumeration for CT-API errorcodes.
*/
/*!
    \var CardException::NoError
    No error, used for passing a message.
*/
/*!
    \var CardException::Invalid
    Invalid parameter or value.
*/
/*!
    \var CardException::CardTerminal
    Cardterminal Error. The terminal is temporary not accessible (busy with other or internal
    processes). The problem can be resolved by the application.
*/
/*!
    \var CardException::Transmission
    Transmission error. Transmission error due to mechanical, electrical or protocol failures.
    Reset of of cardterminal is necessary.
*/
/*!
    \var CardException::Memory
    Memory assignment error. A memory error occurred (the allocated buffer is too small for
    the returned data).
*/
/*!
    \var CardException::HTSI
    Host Transport Service Interface error. Commonly returned if the error is produced by the
    software layer and not in the communication with the hardware.
*/
/*!
    \var CardException::DataCorrupted
    Read Binary: Data corrupted.
*/
/*!
    \var CardException::EndReached
    Read Binary: The specified data is not in the valid range for the chipcard.
*/
/*!
    \var CardException::MemoryFailure
    Read Binary: Memory failure.
*/
/*!
    \var CardException::Error
    General error.
*/
/*!
    \var CardException::WrongVerification
    The verification was wrong. You have to set the number of retries. Only a specified number
    of retries are allowed, othwerwise the card is destroyed.
*/
/*!
    \var CardException::VerificationBlocked
    Verification method blocked
*/

/*!
    Creates a new instance of the exception and includes the error message. This message is
    returned by the what() method. Consider using the other constructor which takes an errorcode.
    \param message the error message
*/
CardException::CardException(const std::string& message)
    : std::runtime_error(message), m_errorcode(NoError), m_retryNumber(-1)
{}


/*!
    Creates a new instance of the exception and sets the error code. The error message
    is generated on the fly if requested.
    \param errorcode the error code, see CardException::ErrorCode.
*/
CardException::CardException(ErrorCode errorcode)
    : std::runtime_error(""), m_errorcode(errorcode), m_retryNumber(-1)
{}


/*!
    Creates a human-readable errorcode message.
    \return the message as English text
*/
const char* CardException::what() const throw ()
{
    switch (m_errorcode)
    {
        case Invalid:
            return "Invalid parameter or value.";
        case CT:
            return "Cardterminal Error. The terminal is temporary not accessible.";
        case Transmission:
            return "Transmission error.";
        case Memory:
            return "Memory assignment error.";
        case HTSI:
            return "Host Transport Service Interface error.";
        case DataCorrupted:
            return "Data corrupted.";
        case EndReached:
            return "The specified data is not in the valid range for the chipcard.";
        case MemoryFailure:
            return "Memory failure.";
        case Error:
            return "Error";
        default:
            char buf[100];
            std::sprintf(buf, "CT-API Errorcode: %x", m_errorcode);
            return (std::string(buf) + std::runtime_error::what()).c_str();
    }
}


/*!
    Returns the error code
    \return the error code or CardException::NoError if a error message but no error code was set
*/
CardException::ErrorCode CardException::getErrorCode() const
{
    return m_errorcode;
}


/*!
    Sets the number of retries. You have to set the number of retries if the exception type
    is CardException::WrongVerification.
*/
void CardException::setRetryNumber(int num)
{
    m_retryNumber = num;
}


/*!
    Returns the number of retries. This value should be set if the type of the exception is
    CardException::WrongVerification. If it was not set, \c -1 is returned.
*/
int CardException::getRetryNumber() const
{
    return m_retryNumber;
}

// :maxLineLen=100:
