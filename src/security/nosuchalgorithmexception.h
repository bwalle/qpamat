/*
 * Id: $Id: nosuchalgorithmexception.h,v 1.5 2003/11/02 20:00:51 bwalle Exp $
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
#ifndef NOSUCHALGORITHMEXCEPTION_H
#define NOSUCHALGORITHMEXCEPTION_H
#include <stdexcept>
#include <string>

/**
 * Exception that is thrown if QPaMaT has not included the necessary algorithm for
 * encryption.
 * @ingroup cipher
 * @version $Revision: 1.5 $
 * @author Bernhard Walle
 * @date $Date: 2003/11/02 20:00:51 $
 */
class NoSuchAlgorithmException : public std::runtime_error
{
    public:
        
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method.
         * @param error the error message
         */
        NoSuchAlgorithmException(const std::string& error) : std::runtime_error(error) { }
        
        /**
         * Deletes the object.
         */
        virtual ~NoSuchAlgorithmException() throw () { }
};

#endif // NOSUCHALGORITHMEXCEPTION_H
