/*
 * Id: $Id: memorycard.h,v 1.3 2003/11/12 22:17:45 bwalle Exp $
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
#include "../types.h"

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
 * Class for handling memory cards. 
 * 
 * Here's some background information about programming with smartcards etc.
 * The information which was used to program this class is from following sources:
 *
 *  - Manual pages ctapi(3) and ctbcs(3) from the Towitoko CT-API driver at 
 *    http://www.geocities.com/cprados/ .
 *  - Official CT-API specification at http://www.panstruga.de/ct-api/spec/spec_v09.html .
 *  - c't (very good German computer magazine) article "Kartenspiele", "Grundlagen der
 *    Chipkartenprogrammierung", Kai-Uwe Mrkor, c't 8/2000, page 208.
 * 
 * A very good overview about the different APIs for programming with chipcards was in the
 * article "Karten-Spiele", "Smartcard-Programmierung unter Linux", Frank Haubenschild in
 * the German magazine "Linux-Magazin" 06/2002 which is also available online at
 * http://www.linux-magazin.de/Artikel/ausgabe/2002/06/smartcards/smartcards.html. I choosed
 * the CT-API because there was a driver available for my reader, this API is wide-spread in
 * Germany, it's available on both *nix and Windows and my homebanking programme Moneyplex
 * from Matrica also uses this API and chipcard communication works well. 
 *
 * There's no need to start a server on machine and the user only has to specify one library.
 * You don't need root permissions if you have access to the serial port, of course.
 * 
 * @ingroup smartcard
 * @author Bernhard Walle
 * @version $Revision: 1.3 $
 * @date $Date: 2003/11/12 22:17:45 $
 */
class MemoryCard
{
    public:
        
        /**
         * The cards in slot 1 to slot 14, used for destination adresses. If the destination is
         * the terminal, use CT from ctapi.h.
         */
        enum CardSlot { ICC1 = 0x00, ICC2 = 0x02, ICC3, ICC4, ICC5, ICC6, ICC7, ICC8, ICC9, ICC10,
            ICC11, ICC12, ICC13, ICC14 };
        
        /**
         * The card type.
         */
        enum CardType
        {
            TMemoryCard,    /*!< Memory card */
            TProcessorCard, /*!< Processor card */
            OtherResponse   /*!< Shows that the program detected no memory card 
                                 or processor card. */
        };
        
        /**
         * The protocol type
         */
        enum ProtocolType
        {
            TI2C,           /*!< I2C */
            T2Wire,         /*!< 2 Wire */
            T3Wire,         /*!< 3 Wire */
            TISOProtocol,   /*!< ISO protocol */
            TOther          /*!< Other type that was not declared in the c't article */
        };
        
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
         * Sets the time that the program waits for the user to insert the card in the terminal.
         * @param newTime the time in seconds
         */
        void setWaitTime(uchar newTime);
        
        /**
         * Returns the time that the program wiats for the user to insert the card in the terminal.
         * @return the time in seconds.
         */
        uchar getWaitTime() const;
        
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
        
        /**
         * Returns the type of the smartcard in the reader. Even though the name of the class is
         * "MemoryCard", this method can also return another type. It was programmed for checking
         * the type before doing anything other.
         */
        CardType getType() const throw (CardException, NotInitializedException);
        
        /**
         * Reads some status information from the chipcard terminal. This information is not
         * card dependand but depends on the terminal.
         * The information is returned by setting the 
         * @param manufacturer 5 characters, the first two are the country code and the following
         *                     3 are the manufacturer acronym
         * @param terminalType the terminal type which is manufacturer dependant
         * @param software     the software version which is manufacturer dependant, too.
         * @param discrData    additional, optional information
         */
        void getStatusInformation(QString* manufacturer, QString* terminalType = 0, 
            QString* software = 0, QString* discrData = 0) 
            throw (CardException, NotInitializedException);
        
        /**
         * Resets the card in slot 1 and returns some information that is returned by the CT-API
         * as response to this action (so-called ATR).
         * @param capacity a pointer to a integer variable where the capacity is stored. If it's
         *                 0, then no value is stored
         * @param protocolType a pointer to a ProtocolType enumeration value where the protocol
         *                     type is stored. If it's 0, then no value is stored
         * @exception CardException if an exception occured while resetting the card
         * @exception NotInitializedException if the object was not initialized*
         */
        void resetCard(int* capacity = 0, ProtocolType* protocolType = 0) const
            throw (NotInitializedException, CardException);;
        
        /**
         * This selects a file on the chipcard. It is implemented in a way that selects the whole
         * data area on the chipcard. The return value of this function indicates the success.
         * @return \c true if the command was successfull, \c false otherwise.
         * @exception CardException if an exception occured while selecting the file
         * @exception NotInitializedException if the object was not initialized
         */
        bool selectFile() const throw (NotInitializedException, CardException);
        
        /**
         * Reads the specified number of bytes from the chipcard.
         * @param offset the offset
         * @param length the number of bytes that should be read
         * @return the read bytes
         * @exception CardException if an exception occured while reading
         * @exception NotInitializedException if the object was not initialized
         */
        ByteVector read(ushort offset, ushort length) const
            throw (CardException, NotInitializedException);
        
        /**
         * Writes the specified data to the smartcard. The data must fit on the card.
         * @param offset the offset where the data should be written
         * @param data the data that should be written
         * @exception CardException if an exception occured while writing
         * @exception NotInitializedException if the object was not initialized
         */
        void write(ushort offset, const ByteVector& data) const 
            throw (CardException, NotInitializedException);
        
        
    protected:
        
        /**
         * Checks if the class was initilized. If not a NotInitializedException is thrown.
         * @exception NotInitializedException if the class was not initialized
         */
        void checkInitialzed(const QString& = QString::null) const throw (NotInitializedException);
        
    private:
        QLibrary m_library;
        CT_init_ptr m_CT_init_function;
        CT_data_ptr m_CT_data_function;
        CT_close_ptr m_CT_close_function;
        ushort m_portNumber;           // pn in CT-API jargon
        ushort m_cardTerminalNumber;   // ctnin CT-API jargon
        bool m_initialized;
        uchar m_waitTime;
        static int m_lastNumber;
};

#endif // MEMORYCARD_H
