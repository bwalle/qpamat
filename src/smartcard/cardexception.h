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
#ifndef CARDEXCEPTION_H
#define CARDEXCEPTION_H

#include <stdexcept>
#include <string>

#include "ctapi.h"

class CardException : public std::runtime_error
{
    public:
        enum ErrorCode 
        {
            NoError             = 0,
            Invalid             = ERR_INVALID,
            CardTerminal        = ERR_CT,
            Transmission        = ERR_TRANS,
            Memory              = ERR_MEMORY,
            HTSI                = ERR_HTSI,
            DataCorrupted       = 0x6281,
            EndReached          = 0x6282,
            MemoryFailure       = 0x6501,
            Error               = 0x6200,
            WrongVerification   = 0x63C0,
            VerificationBlocked = 0x6983
        };
    
    public:
        CardException(const std::string& error);
        CardException(ErrorCode errorcode);
        
        const char* what() const throw ();
        ErrorCode getErrorCode() const;
        
        void setRetryNumber(int num);
        int getRetryNumber() const;
    
    private:
        ErrorCode   m_errorcode;
        int         m_retryNumber;
};

#endif // CARDEXCEPTION_H

// :maxLineLen=100:
