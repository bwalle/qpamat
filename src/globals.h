/*
 * Id: $Id: globals.h,v 1.2 2003/10/12 15:11:26 bwalle Exp $
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
#ifndef GLOBALS_H
#define GLOBALS_H

#include <qdir.h>

/** 
 * @defgroup cipher Cipher
 * Classes used for encryption, decryption and related things.
 */

/**
 * @defgroup gui GUI
 * Classes for GUI
 */

/**
 * @file globals.h
 * This file contains application-wide constants.
 */

/**
 * User-visible string for the application name. Used for the window title
 * in error messages, for example.
 */
#define QPAMAT_USERVISIBLE "QPaMaT"

/**
 * The default file where the passwords are stored.
 */
#define QPAMAT_FILE_NAME (QDir::home().absPath()+"/.qpamat")

#endif // GLOBALS_H
