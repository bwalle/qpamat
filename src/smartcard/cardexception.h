/*
 * Id: $Id: cardexception.h,v 1.2 2003/11/12 22:17:57 bwalle Exp $
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
#ifndef CARDEXCEPTION_H
#define CARDEXCEPTION_H

#include <stdexcept>
#include <string>

#include "ctapi.h"

/**
 * This exception is thrown if an error occured while communicating with the smartcard
 * terminal. It just encapsulates the error codes of CT-API in a programmer-friendly
 * way.
 * @ingroup smartcard
 * @author Bernhard Walle
 * @version $Revision: 1.2 $
 * @date $Date: 2003/11/12 22:17:57 $
 */
class CardException : public std::runtime_error
{
    public:
        
        /** 
         * Enumeration for CT-API errorcodes.
         */
        enum ErrorCode 
        {
            NoError       = 0,           /*!< No error, used for passing a message. */
            Invalid       = ERR_INVALID, /*!< Invalid parameter or value. */ 
            CardTerminal  = ERR_CT,      /*!< Cardterminal Error. The terminal is temporary not
                                            accessible (busy with other or internal processes).
                                            The problem can be resolved by the application. */
            Transmission  = ERR_TRANS,   /*!< Transmission error. Transmission error due to
                                            mechanical, electrical or protocol failures. Reset of
                                            of cardterminal is necessary. */
            Memory        = ERR_MEMORY,  /*!< Memory assignment error. A memory error occured (the
                                            allocated buffer is too small for the returned data).*/
            HTSI          = ERR_HTSI,    /*!< Host Transport Service Interface error. Commonly re-
                                            turned if the error is produced by the software layer
                                            and not in the communication with the hardware. */
            DataCorrupted = 0x6281,      /*!< Read Binary: Data corrupted. */
            EndReached    = 0x6282,      /*!< Read Binary: The specified data is not in the valid
                                            range for the chipcard. */
            MemoryFailure = 0x6501,      /*!< Read Binary: Memory failure. */
            Error         = 0x6200       /*!< General error. */
        };
            
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method. Consider using the other constructor
         * which takes a 
         * @param error the error message
         */
        CardException(const std::string& error);
        
        /**
         * Creates a new instance of the exception and sets the error code. The error message
         * is generated on the fly if requested.
         */
        CardException(ErrorCode errorcode);
        
        /**
         * Returns the error message.
         * @return the error message (in English)
         */
        virtual const char* what() const throw ();
        
        /**
         * Returns the error code.
         * @return the error code
         */
        ErrorCode getErrorCode() const;
    
    private:
        ErrorCode m_errorcode;
};


#endif // CARDEXCEPTION_H
