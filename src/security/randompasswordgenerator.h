/*
 * Id: $Id: randompasswordgenerator.h,v 1.8 2003/12/18 22:00:12 bwalle Exp $
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
#ifndef RANDOMPASSWORDGENERATOR_H
#define RANDOMPASSWORDGENERATOR_H

#include <qstring.h>

#include "../global.h"
#include "passwordgenerator.h"

/*!
 * \brief Object to generate random passwords. 
 *
 * The passwords are \b really random since it is used the OpenSSL library for randomness.
 *
 * \ingroup security
 * \author Bernhard Walle
 * \version $Revision: 1.8 $
 * \date $Date: 2003/12/18 22:00:12 $
 */
class RandomPasswordGenerator : public PasswordGenerator
{
    public:
        
        /*!
         * Generates a random password. There's a random password generated using a
         * function of the OpenSSL library. This password consists of bytes. The
         * bytes are transformed to a string using the base64 algorithm, which
         * ensures that there could be no charset conversion problem.
         * The passwords are not easy to memorize but you have QPaMaT and your clipboard.
         * However, if you need a good password to memorize, just use your brain. A
         * computer cannot output what is easy to memorize for <b>you</b>!
         * \param length the length of the password
         * \param allowedChars list of all allowed characters or an empty list if all chars are
         *                     allowed
         * \return the password
         * \exception PasswordGenerateException if the object was not seeded
         */
        virtual QString getPassword(uint length, QCharVector allowedChars = QCharVector()) 
            throw (PasswordGenerateException);
        
        /*!
         * Returns if the object was seeded. On Linux or other operating systems with
         * a /dev/urandom device this is done automatically, on other system this
         * must be done with user input. (System time or something else is not
         * enouth.)
         */
        virtual bool isSeeded();
        
        /*!
         * Returns \c false
         * \return \c false
         */
        bool isSlow() const;
};

#endif // RANDOMPASSWORDGENERATOR_H
