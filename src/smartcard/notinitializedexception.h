/*
 * Id: $Id: notinitializedexception.h,v 1.2 2003/11/10 23:17:40 bwalle Exp $
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
#ifndef NOTINITIALIZEDEXCEPTION_H
#define NOTINITIALIZEDEXCEPTION_H

#include <stdexcept>
#include <string>

#include <qstring.h>

/**
 * Exception that is thrown if the object was not initalized using the init() method.
 * @ingroup smartcard
 * @version $Revision: 1.2 $
 * @author Bernhard Walle
 * @date $Date: 2003/11/10 23:17:40 $
 */
class NotInitializedException : public std::runtime_error
{
    public:
        
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method.
         * @param error the error message
         */
        NotInitializedException(const std::string& error) : std::runtime_error(error) { }
        
        
        /**
         * Creates a new instance of the exception and includes the error message. This
         * message is returned by the what() method. It behaves exactly like the above
         * constructor and is provided only for convenience.
         * @param error the error message
         */
        NotInitializedException(const QString& error) : std::runtime_error(error.latin1()) { }
        
        /**
         * Deletes the object.
         */
        virtual ~NotInitializedException() throw () { }
};

#endif // NOTINITIALIZEDEXCEPTION_H
