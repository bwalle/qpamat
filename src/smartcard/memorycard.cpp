/*
 * Id: $Id: memorycard.cpp,v 1.3 2003/11/12 22:17:45 bwalle Exp $
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

#include "nosuchlibraryexception.h"
#include "memorycard.h"
#include "cardexception.h"
#include "../types.h"

// -------------------------------------------------------------------------------------------------
int MemoryCard::m_lastNumber = 1;
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
MemoryCard::MemoryCard(QString library) throw (NoSuchLibraryException)
// -------------------------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------------------------
MemoryCard::~MemoryCard()
// -------------------------------------------------------------------------------------------------
{
    if (m_initialized)
    {
        try
        {
            close();
        }
        catch (const std::exception& ex)
        {
            qDebug(ex.what());
        }
        catch (...)
        {}
    }
}


// -------------------------------------------------------------------------------------------------
void MemoryCard::init(int portNumber) throw (CardException)
// -------------------------------------------------------------------------------------------------
{
    if (m_initialized)
    {
        qDebug("init() before close()");
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


// -------------------------------------------------------------------------------------------------
void MemoryCard::close() throw (CardException, NotInitializedException)
// -------------------------------------------------------------------------------------------------
{
    checkInitialzed("close");
    
    char ret = m_CT_close_function(m_cardTerminalNumber);
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    m_initialized = false;
}


// -------------------------------------------------------------------------------------------------
uchar MemoryCard::getWaitTime() const
// -------------------------------------------------------------------------------------------------
{
    return m_waitTime;
}


// -------------------------------------------------------------------------------------------------
void MemoryCard::setWaitTime(uchar newTime)
// -------------------------------------------------------------------------------------------------
{
    m_waitTime = newTime;
}


// -------------------------------------------------------------------------------------------------
MemoryCard::CardType MemoryCard::getType() const
// -------------------------------------------------------------------------------------------------
        throw (CardException, NotInitializedException)
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


// -------------------------------------------------------------------------------------------------
void MemoryCard::resetCard(int* capacity, ProtocolType* protocolType) const
// -------------------------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------------------------
void MemoryCard::getStatusInformation(QString* manufacturer, QString* terminalType, 
        QString* software, QString* discrData)
// -------------------------------------------------------------------------------------------------
        throw (CardException, NotInitializedException)
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


// -------------------------------------------------------------------------------------------------
bool MemoryCard::selectFile() const
// -------------------------------------------------------------------------------------------------
        throw (CardException, NotInitializedException)
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


// -------------------------------------------------------------------------------------------------
ByteVector MemoryCard::read(ushort offset, ushort length) const
// -------------------------------------------------------------------------------------------------
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
        qDebug("Could not read all requested data. Maybe problem?"); // TODO: remove this
        throw CardException(CardException::EndReached);
    }
    
    return vec;
}


// -------------------------------------------------------------------------------------------------
void MemoryCard::write(ushort offset, const ByteVector& data) const
// -------------------------------------------------------------------------------------------------
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
            throw CardException(CardException::ErrorCode(ret));
        }
        
        ushort sw1sw2 = (response[lenr-2] << 8) + response[lenr-1];
        
        if (sw1sw2 != 0x9000)
        {
            throw CardException(CardException::ErrorCode(sw1sw2));
        }
        
        len -= max;
        dataOffset += max;
    }
}


// -------------------------------------------------------------------------------------------------
void MemoryCard::checkInitialzed(const QString& functionName) const
// -------------------------------------------------------------------------------------------------
        throw (NotInitializedException)
{
    if (!m_initialized)
    {
        throw NotInitializedException("The function " + functionName + " was called even though "
            "the class was not initalized.");
    }
}
