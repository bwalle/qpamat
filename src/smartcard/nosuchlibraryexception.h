/*
 * Id: $Id: nosuchlibraryexception.h,v 1.1 2003/11/04 22:49:06 bwalle Exp $
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
#ifndef NOSUCHLIBRARYEXCEPTION_H
#define NOSUCHLIBRARYEXCEPTION_H

#include <stdexcept>
#include <string>

#include <qstring.h>

/**
 * Exception that is thrown if the specified library does not exist.
 * @ingroup smartcard
 * @version $Revision: 1.1 $
 * @author Bernhard Walle
 * @date $Date: 2003/11/04 22:49:06 $
 */
class NoSuchLibraryException : public std::runtime_error
{
    public:
        
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method.
         * @param error the error message
         */
        NoSuchLibraryException(const std::string& error) : std::runtime_error(error) { }
        
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method. It behaves exactly like the above
         * constructor and is provided only for convenience.
         * @param error the error message
         */
        NoSuchLibraryException(const QString& error) : std::runtime_error(error.latin1()) { }
        
        /**
         * Deletes the object.
         */
        virtual ~NoSuchLibraryException() throw () { }
};

#endif // NOSUCHLIBRARYEXCEPTION_H
