/*
 * Id: $Id: cardexception.cpp,v 1.2 2003/11/12 22:17:57 bwalle Exp $
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
#include <stdexcept>
#include <string>

#include "cardexception.h"

// -------------------------------------------------------------------------------------------------
CardException::CardException(const std::string& message)
// -------------------------------------------------------------------------------------------------
        : std::runtime_error(message), m_errorcode(NoError)
{}


// -------------------------------------------------------------------------------------------------
CardException::CardException(ErrorCode errorcode)
// -------------------------------------------------------------------------------------------------
        : std::runtime_error(""), m_errorcode(errorcode)
{}

// -------------------------------------------------------------------------------------------------
const char* CardException::what() const throw ()
// -------------------------------------------------------------------------------------------------
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
            return std::runtime_error::what();
    }
}


// -------------------------------------------------------------------------------------------------
CardException::ErrorCode CardException::getErrorCode() const
// -------------------------------------------------------------------------------------------------
{
    return m_errorcode;
}
