/*
 * Id: $Id: memorycard.cpp,v 1.9 2004/01/13 23:48:54 bwalle Exp $
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
#include <string>
#include <cmath>
#include <algorithm>

#include <qstring.h>
#include <qlibrary.h>
#include <qapplication.h>

#include "memorycard.h"
#include "global.h"
#include "nosuchlibraryexception.h"
#include "cardexception.h"

// -------------------------------------------------------------------------------------------------
//                                     Static variables
// -------------------------------------------------------------------------------------------------

int MemoryCard::m_lastNumber = 1;


/*!
    \class MemoryCard
    
    \brief Class for handling memory cards.
    
    Here's some background information about programming with smartcards etc.
    The information which was used to program this class is from following sources:
    
     - Manual pages ctapi(3) and ctbcs(3) from the Towitoko CT-API driver at 
       http://www.geocities.com/cprados/ .
     - Official CT-API specification at http://www.panstruga.de/ct-api/spec/spec_v09.html .
     - c't (very good German computer magazine) article "Kartenspiele", "Grundlagen der
       Chipkartenprogrammierung", Kai-Uwe Mrkor, c't 8/2000, page 208.
    
    A very good overview about the different APIs for programming with chipcards was in the
    article "Karten-Spiele", "Smartcard-Programmierung unter Linux", Frank Haubenschild in
    the German magazine "Linux-Magazin" 06/2002 which is also available online at
    http://www.linux-magazin.de/Artikel/ausgabe/2002/06/smartcards/smartcards.html. I choosed
    the CT-API because there was a driver available for my reader, this API is wide-spread in
    Germany, it's available on both *nix and Windows and my homebanking programme Moneyplex
    from Matrica also uses this API and chipcard communication works well.
    
    There's no need to start a server on machine and the user only has to specify one library.
    You don't need root permissions if you have access to the serial port, of course.
    
    \par Normal Usage:
    
     - At first you have to create a new object,
     - then you have to call the init() method with the right port. 
     - After that you have to request the type of the card.
     - Now you can call the read() or write() method.
     - After this you have to call the close() method or simply let the destructor do the work!
    
    \ingroup smartcard
    \author Bernhard Walle
    \version $Revision: 1.9 $
    \date $Date: 2004/01/13 23:48:54 $
*/

/*!
    \typedef char (*MemoryCard::CT_init_ptr) (ushort ctn, ushort pn);
    
    Typedef for the init function of the CT-API.
*/

/*!
    \typedef typedef char (*MemoryCard::CT_data_ptr) (ushort ctn, uchar* dad, uchar* sad, ushort lenc, 
                                          uchar* command, ushort* lenr, uchar* response);
    
    Typedef for the data function of the CT-API.
*/

/*!
    \typedef char (*MemoryCard::CT_close_ptr) (ushort ctn);
    
    Typedef for the close function of the CT-API.
*/

/*!
    \enum MemoryCard::CardSlot
    
    The cards in slot 1 to slot 14, used for destination adresses. If the destination is
    the terminal, use CT from ctapi.h.
*/

/*!
    \enum MemoryCard::CardType
    
    The card type.
*/
/*!
    \var MemoryCard::TMemoryCard
    Memory card
*/
/*!
    \var MemoryCard::TProcessorCard
    Processor card
*/
/*!
    \var MemoryCard::OtherResponse
    Shows that the program detected no memory card or processor card.
*/

/*!
    \enum MemoryCard::ProtocolType
    
    The protocol type.
*/


/*!
     Creates a new instance of MemoryCard.
     \param library path to the CT-API library
     \exception NoSuchLibraryException if the loading of the library failed
*/
MemoryCard::MemoryCard(QString library) throw (NoSuchLibraryException)
    : m_library(library), m_initialized(false), m_waitTime(0)
{
    if (! m_library.load())
    {
        throw NoSuchLibraryException("The library \""+ library + "\" could not be loaded." );
    }
    
    // initialize the functions ...
    m_CT_init_function = (CT_init_ptr)m_library.resolve("CT_init");
    m_CT_data_function = (CT_data_ptr)m_library.resolve("CT_data");
    m_CT_close_function = (CT_close_ptr)m_library.resolve("CT_close");
    
    if (!m_CT_init_function)
    {
        throw NoSuchLibraryException("\""+ library +"\" is not a CT-API driver. It lacks the "
            " CT_init() function.");
    }
    if (!m_CT_data_function)
    {
        throw NoSuchLibraryException("\""+ library +"\" is not a CT-API driver. It lacks the "
            " CT_data() function.");
    }
    if (!m_CT_close_function)
    {
        throw NoSuchLibraryException("\""+ library +"\" is not a CT-API driver. It lacks the "
            " CT_close() function.");
    } 
}


/*!
    Deletes a MemoryCard object. Calls the close() method if this was not done before. This is very
    important for exception handling!
*/
MemoryCard::~MemoryCard()
{
    if (m_initialized)
    {
        try
        {
            close();
        }
        catch (const std::exception& ex)
        {
            PRINT_DBG("Caught exception in destructor: %s", ex.what()); 
        }
        catch (...)
        {
            PRINT_DBG("Caught general exception in destructor.");
        }
    }
}


/*!
    Initializes the MemoryCard object for reading from or writing to a smart card
    reader. You \b must call this function before accessing any other operations.
    \param portNumber the port number. According to CT-API the mapping between
                      port number and real hardware port is manufactor specific.
                      There are some defines in ctapi.h like COM_1 which may work or
                      may not work. On Unix often 1 is mapped to COM1 (/dev/ttyS0), on 
                      Windows it is COM2. For USB or PS/2 there's no rule at all. Just
                      let the user test all possibilities. :-)
    \exception CardException if an exception occurred
*/
void MemoryCard::init(int portNumber) throw (CardException)
{
    if (m_initialized)
    {
        qWarning("init() before close()");
    }
    
    char ret = m_CT_init_function(m_lastNumber, portNumber);
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    
    m_portNumber = portNumber;
    m_cardTerminalNumber = m_lastNumber++;
    m_initialized = true;
}


/*!
    Closes the MemoryCard communtion. This method must be called after the communction.
    \exception CardException if an exception occurred
    \exception NotInitializedException if the object was not initialized
*/
void MemoryCard::close() throw (CardException, NotInitializedException)
{
    checkInitialzed("close");
    
    char ret = m_CT_close_function(m_cardTerminalNumber);
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    m_initialized = false;
}


/*!
    Returns the time that the program wiats for the user to insert the card in the terminal.
    \return the time in seconds.
*/
uchar MemoryCard::getWaitTime() const
{
    return m_waitTime;
}


/*!
    Sets the time that the program waits for the user to insert the card in the terminal.
    \param newTime the time in seconds
*/
void MemoryCard::setWaitTime(uchar newTime)
{
    m_waitTime = newTime;
}


/*!
    Returns the type of the smartcard in the reader. Even though the name of the class is
    "MemoryCard", this method can also return another type. It was programmed for checking
    the type before doing anything other.
*/
MemoryCard::CardType MemoryCard::getType() const throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    //                     CLA   INS   P1    P2    LEN  WAIT
    byte REQUEST_ICC[] = { 0x20, 0x12, 0x01, 0x00, 1,  m_waitTime };
    
    byte sad = HOST;      // source
    byte dad = CT;        // destination
    
    byte response[100];
    ushort lenr = sizeof(response);
    
    char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, sizeof(REQUEST_ICC), 
        REQUEST_ICC, &lenr, response);
    
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    
    ushort sw1sw2 = (response[0] << 8) + response[1];
    
    switch (sw1sw2)
    {
        case 0x9000: return TMemoryCard;
        case 0x9001: return TProcessorCard;
        default:     return OtherResponse;
    }
}


/*!
    Resets the card in slot 1 and returns some information that is returned by the CT-API
    as response to this action (so-called ATR).
    \param capacity a pointer to a integer variable where the capacity is stored. If it's
                    0, then no value is stored
    \param protocolType a pointer to a ProtocolType enumeration value where the protocol
                        type is stored. If it's 0, then no value is stored
    \exception CardException if an exception occurred while resetting the card
    \exception NotInitializedException if the object was not initialized
*/
void MemoryCard::resetCard(int* capacity, ProtocolType* protocolType) const
    throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    //                  CLA   INS   P1    P2    LEN
    byte RESET_CT[] = { 0x20, 0x11, 0x01, 0x01, 0};
    
    byte sad = HOST;      // source
    byte dad = CT;        // destination
    
    byte response[100];
    ushort lenr = sizeof(response);
    
    char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, sizeof(RESET_CT), 
        RESET_CT, &lenr, response);
    
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    
    
    if (lenr < 4)
    {
        qDebug("Return length is not 2, this is invalid. It was %d.\n", lenr);
        if (capacity != 0)
        {
            *capacity = 0;
        }
        if (protocolType != 0)
        {
            *protocolType = TOther;
        }
    }
    
    
    byte h1 =  response[0]; // protocol type
    byte protocol = (h1 >> 4) & 0x0F;
    
    if (protocolType != NULL)
    {
        if (protocol < 0x08)
        {
             *protocolType = TISOProtocol;
        }
        else
        {
            switch (protocol)
            {
                case 0x08:
                    *protocolType = TI2C;
                    break;
                case 0x09:
                    *protocolType = T2Wire;
                    break;
                case 0x0A:
                    *protocolType = T3Wire;
                    break;
                default:
                    *protocolType = TOther;
                    break;
            }
        };
    }
    
    if (capacity != 0)
    {
        byte h2 = response[1]; // size
        
        int number = ((h2 >> 3) & 0x0F);
        if (number > 0)
        {
            number = 1 << number + 6; // 2^{number+6}
        }
        int blocksize = 1 << (h2 & 0x07);
        *capacity = number * blocksize / 8;
    }
}


/*!
    Reads some status information from the chipcard terminal. This information is not
    card dependand but depends on the terminal. The information is returned by setting 
    the paramters to a sensible value.
    \param manufacturer 5 characters, the first two are the country code and the following
                        3 are the manufacturer acronym
    \param terminalType the terminal type which is manufacturer dependant
    \param software     the software version which is manufacturer dependant, too.
    \param discrData    additional, optional information
*/
void MemoryCard::getStatusInformation(QString* manufacturer, QString* terminalType, 
        QString* software, QString* discrData) throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    
    //                        CLA   INS   P1    P2    LEN
    byte REQUEST_STATUS[] = { 0x20, 0x13, 0x00, 0x46, 0 };
    
    byte sad = HOST;      // source
    byte dad = CT;        // destination
    
    byte response[100];
    ushort lenr = sizeof(response);
    
    char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, sizeof(REQUEST_STATUS), 
        REQUEST_STATUS, &lenr, response);
    
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    
    QString resp = QString::fromLatin1(reinterpret_cast<char*>(response), lenr);
    
    if (manufacturer != 0)
    {
        *manufacturer = resp.left(5);
    }
    
    if (terminalType != 0)
    {
        *terminalType = resp.mid(5, 5);
    }
    
    if (software != 0)
    {
        *software = resp.mid(10, 5);
    }
    
    if (discrData != 0)
    {
        *discrData = resp.right(lenr - 15);
    }
}


/*!
    This selects a file on the chipcard. It is implemented in a way that selects the whole
    data area on the chipcard. The return value of this function indicates the success. 
    \return \c true if the command was successfull, \c false otherwise.
    \exception CardException if an exception occurred while selecting the file
    \exception NotInitializedException if the object was not initialized
*/
bool MemoryCard::selectFile() const throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    //                     CLA   INS   P1    P2    LEN -- total --
    byte SELECT_FILE[] = { 0x00, 0xA4, 0x00, 0x00, 2,  0x3F, 0x00 };

    byte sad = HOST;      // source
    byte dad = ICC1;         // destination
    
    
    byte response[2];
    ushort lenr = sizeof(response);
    
    char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, sizeof(SELECT_FILE), 
        SELECT_FILE, &lenr, response);
    
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    
    ushort sw1sw2 = (response[0] << 8) + response[1];
    
    return sw1sw2 == 0x9000;
}


/*!
    Reads the specified number of bytes from the chipcard.
    \param offset the offset
    \param length the number of bytes that should be read
    \return the read bytes
    \exception CardException if an exception occurred while reading
    \exception NotInitializedException if the object was not initialized
*/
ByteVector MemoryCard::read(ushort offset, ushort length) 
    throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    byte read_binary[5];
    read_binary[0] = 0x00; // CLA
    read_binary[1] = 0xB0; // INS
    
    ByteVector vec(length);
    int readBytes = 0;
    
    int dataOffset = 0;
    int stillToRead = length;
    const int max = 255;
    
    while (stillToRead > 0)
    {
        int dataToRead = std::min(stillToRead, max);
        
        read_binary[2] = (offset + dataOffset) >> 8;
        read_binary[3] = (offset + dataOffset) & 0xFF;
        read_binary[4] = dataToRead;
        
        byte sad = HOST;      // source
        byte dad = ICC1;      // destination
        
        byte response[max+2];
        ushort lenr = dataToRead + 2;
        
        char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, sizeof(read_binary), 
            read_binary, &lenr, response);
        
        if (ret != OK)
        {
            throw CardException(CardException::ErrorCode(ret));
        }
        
        ushort sw1sw2 = (response[lenr-2] << 8) + response[lenr-1];
        
        if (sw1sw2 != 0x9000)
        {
            throw CardException(CardException::ErrorCode(sw1sw2));
        }
        
        readBytes += lenr - 2;
        qCopy(response, response + lenr - 2, vec.begin() + dataOffset);
        
        stillToRead -= max;
        dataOffset += max;
    }
    
    // truncate if not all could be read
    vec.resize(readBytes);
    if (readBytes != length)
    {
        throw CardException(CardException::EndReached);
    }
    
    return vec;
}


/*!
    Writes the specified data to the smartcard. The data must fit on the card.
    \param offset the offset where the data should be written
    \param data the data that should be written
    \exception CardException if an exception occurred while writing
    \exception NotInitializedException if the object was not initialized
*/
void MemoryCard::write(ushort offset, const ByteVector& data)
    throw (CardException, NotInitializedException)
{
    checkInitialzed();
    
    int dataOffset = 0;
    int len = data.size();
    const int max = 255;
    
    byte update_binary[max+5];
    update_binary[0] = 0x00; // CLA
    update_binary[1] = 0xD6; // INS
        
    while (len > 0)
    {
        int written_bytes = std::min(len, max);
        
        update_binary[2] = (offset + dataOffset) >> 8; // P1
        update_binary[3] = (offset + dataOffset) & 0xFF; // P2
        update_binary[4] = written_bytes; // LEN
        
        // fill the array
        ByteVector::const_iterator realBegin = data.begin() + dataOffset;
        
        qCopy(realBegin, realBegin + written_bytes, update_binary + 5);
        
        byte sad = HOST;      // source
        byte dad = ICC1;      // destination
        
        byte response[100];
        ushort lenr = sizeof(response);
        
        char ret = m_CT_data_function(m_cardTerminalNumber, &dad, &sad, written_bytes+5, 
            update_binary, &lenr, response);
        
        if (ret != OK)
        {
            PRINT_DBG("Throwing exception with error code %d", ret)
            throw CardException(CardException::ErrorCode(ret));
        }
        
        ushort sw1sw2 = (response[lenr-2] << 8) + response[lenr-1];
        
        if (sw1sw2 != 0x9000)
        {
            PRINT_DBG("Throwing card exception with error code %x", sw1sw2);
            throw CardException(CardException::ErrorCode(sw1sw2));
        }
        
        len -= max;
        dataOffset += max;
    }
}


/*!
    Checks if the class was initilized. If not a NotInitializedException is thrown.
    \exception NotInitializedException if the class was not initialized
*/
void MemoryCard::checkInitialzed(const QString& functionName) const
    throw (NotInitializedException)
{
    if (!m_initialized)
    {
        throw NotInitializedException("The function " + functionName + " was called even though "
            "the class was not initalized.");
    }
}

