/*
 * Id: $Id: main.h,v 1.2 2003/12/14 19:19:50 bwalle Exp $
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
#ifndef MAIN_H
#define MAIN_H

/**
 * \brief This namespace contains all functions that are no member functions of a class.
 *
 * In my opinion it makes no sense to put helping functions that are called in the main()
 * function in a extra class so I created this namespace.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/14 19:19:50 $
 */
namespace qpamatNS
{
    /*!
     * Parses the command line and calls the right functions. Call this function after
     * creating a QApplication.
     * \param argc the number of arguments
     * \param an array of strings
     */
    void parseCommandLine(int& argc, char**& argv);
    
    /*!
     * Prints the available command line options on stdout and exits the program.
     */
    void printCommandlineOptions();
    
    /*!
     * Prints the version of the program on stderr and exits the program.
     */
    void printVersion();
    
    /*!
     * This function is to ensure that only one instance of Qpamat is running. It must be
     * called before the real application is created (especially before any configuration file
     * is read).
     */
    void singleAppStartup();
    
    /*!
     * This function is to delete the lockfile.
     */
    void singleAppEnd();
    
    /*!
     * Returns the X11 version if the code is compiled on a system with X11. If not,
     * the strings are not changed.
     * \param protocolVersion the version of the X11 protocol, normally 11
     * \param vendorVersion the version of the implementation e.g. of XFree86 on Linux
     */
    void getX11Version(QString& protocolVersion, QString& vendorVersion);
};


#endif // MAIN_H
