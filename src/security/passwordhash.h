/*
 * Id: $Id: passwordhash.h,v 1.2 2003/11/28 18:41:41 bwalle Exp $
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
#ifndef PASSWORDHASH_H
#define PASSWORDHASH_H

#include <qstring.h>

#include "encryptor.h"

/**
 * Helping functions for dealing with passwords and hashes.
 * @ingroup security
 * @author Bernhard Walle
 * @version $Revision: 1.2 $
 * @date $Date: 2003/11/28 18:41:41 $
 */
class PasswordHash
{
    public:
        
        /**
         * Checks the given password if it is the same as the one stored in the hash.
         * (There's no way to re-construct the password from the hash. There's a hash
         * generated from the password and this one is compared to that which was
         * provided by \p hash.)
         * @param password the password
         * @param hash the hash object
         * @return \c true if the password is correct, \c false otherwise
         */
        static bool isCorrect(QString password, QString hash);
        
        /**
         * Generates a hash from the password. That hash contains a salt, too, which
         * makes storing and veryfiing hashes more secure. To be more precisely, a
         * so-called dictinoary attack is prepended which this method because the
         * dictionary would be very large.
         */
        static QString generateHash(QString password);
    
    private:
        static void attachHashWithoutSalt(ByteVector& output, const QString& password);
    
    private:
        static const int numberOfRandomBits;
};

#endif // PASSWORDHASH_H
