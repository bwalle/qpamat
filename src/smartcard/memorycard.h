/*
 * Id: $Id: memorycard.h,v 1.1 2003/11/04 22:49:06 bwalle Exp $
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
#ifndef MEMORYCARD_H
#define MEMORYCARD_H

#include <qstring.h>
#include <qlibrary.h>

#include "nosuchlibraryexception.h"
#include "notinitializedexception.h"
#include "cardexception.h"

/**
 * The init function of the CT-API
 */
typedef char (*CT_init_ptr) (ushort ctn, ushort pn);

/**
 * The data function of the CT-API.
 */
typedef char (*CT_data_ptr) (ushort ctn, uchar* dad, uchar* sad, ushort lenc, 
                             uchar* command, ushort* lenr, uchar* response);

/**
 * Then end function of the CT-API
 */
typedef char (*CT_close_ptr) (ushort ctn);


/**
 * Class for handling memory cards
 * @ingroup smartcard
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/11/04 22:49:06 $
 */
class MemoryCard
{
    public:
        
        /**
         * Creates a new instance of MemoryCard.
         * @param library path to the CT-API library
         * @exception NoSuchLibraryException if the loading of the library failed
         */
        MemoryCard(QString library) throw (NoSuchLibraryException);
        
        /**
         * Deletes a MemoryCard object. Calls the close() method if this was not done before
         * but it's not good programming style to let this do the destructur. So: Do it yourself
         * before!
         */
        virtual ~MemoryCard();
        
        /**
         * Initializes the MemoryCard object for reading from or writing to a smart card
         * reader. You \b must call this function before accessing any other operations.
         * @param portNumber the port number. According to CT-API the mapping between
         *                   port number and real hardware port is manufactor specific.
         *                   There are some defines in ctapi.h like COM_1 which may work or
         *                   may not work. On Unix often 1 is mapped to COM1 (/dev/ttyS0), on 
         *                   Windows it is COM2. For USB or PS/2 there's no rule at all. Just
         *                   let the user test all possibilities. :-)
         * @exception CardException if an exception occured
         */
        void init(int portNumber) throw (CardException);
        
        /**
         * Closes the MemoryCard communtion. This method must be called after the communction.
         * @exception CardException if an exception occured
         * @exception NotInitializedException if the object was not initialized
         */
        void close() throw (CardException, NotInitializedException);
        
    private:
        QLibrary m_library;
        CT_init_ptr m_CT_init_function;
        CT_data_ptr m_CT_data_function;
        CT_close_ptr m_CT_close_function;
        int m_portNumber;           // pn in CT-API jargon
        int m_cardTerminalNumber;   // ctnin CT-API jargon
        static int m_lastNumber;
};

#endif // MEMORYCARD_H
