/*
 * Id: $Id: constants.h,v 1.1 2003/09/21 16:01:35 bwalle Exp $
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
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <openssl/evp.h>

/**
 * @file constants.h
 * This file contains some constants for the cryptographic algorithms used in this program.
 */

 /**
 * The hash algorithm used in this program. See also "Abenteuer Kryptologie", page 292
 * for a detailed description of the security of such algorithms. According to
 * this book and according to my interpretion this is the best one. If you change
 * this the password could not be read by a "normal" compiled program, so be careful.
 */
#define HASH_ALGORITHM EVP_ripemd160()

/**
 * A higher count makes the algorithm more secure but is slower.
 * See http://www.openssl.org/docs/crypto/EVP_BytesToKey.html.
 */
#define KEY_CREATION_COUNT 5


#endif // CONSTANTS_H
