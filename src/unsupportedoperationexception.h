/*
 * Id: $Id: unsupportedoperationexception.h,v 1.1 2003/12/10 21:49:23 bwalle Exp $
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
#ifndef UNSUPPORTEDOPERATIONEXCEPTION_H
#define UNSUPPORTEDOPERATIONEXCEPTION_H

#include <stdexcept>
#include <string>

/*!
 * \brief Exception that is thrown if an operation is not supported as a special exception.
 * The idea is taken from Java's UnsupportedOperationException in the collection classes.
 *
 * \ingroup misc
 * \version $Revision: 1.1 $
 * \author Bernhard Walle
 * \date $Date: 2003/12/10 21:49:23 $
 */
class UnsupportedOperationException : public std::logic_error
{
    public:
        
        /*!
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method.
         * \param error the error message
         */
        UnsupportedOperationException(const std::string& error) : std::logic_error(error) { }
        
        /*!
         * Deletes the object.
         */
        virtual ~UnsupportedOperationException() throw () { }
};

#endif // UNSUPPORTEDOPERATIONEXCEPTION_H
