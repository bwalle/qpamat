/*
 * Id: $Id: passwordcheckexception.h,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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
#ifndef PASSWORDCHECKEXCEPTION_H
#define PASSWORDCHECKEXCEPTION_H

#include <stdexcept>
#include <string>

/*!
 * \brief Exception that is thrown if password checking failed.
 * 
 * There can be different reasons for this. One reason could be that executing an external
 * program failed. Another reason could be that a specified file does not exist.
 *
 * \ingroup security
 * \version $Revision: 1.1 $
 * \author Bernhard Walle
 * \date $Date: 2003/12/04 11:56:05 $
 */
class PasswordCheckException : public std::runtime_error
{
    public:
        
        /*!
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method.
         * \param error the error message
         */
        PasswordCheckException(const std::string& error) : std::runtime_error(error) { }
        
        /*!
         * Deletes the object.
         */
        virtual ~PasswordCheckException() throw () { }
};

#endif // PASSWORDCHECKEXCEPTION_H
