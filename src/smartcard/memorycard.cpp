/*
 * Id: $Id: memorycard.cpp,v 1.1 2003/11/04 22:49:06 bwalle Exp $
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
#include <qstring.h>
#include <qlibrary.h>
#include <string>

#include "nosuchlibraryexception.h"
#include "memorycard.h"
#include "cardexception.h"

// -------------------------------------------------------------------------------------------------
int MemoryCard::m_lastNumber = 1;
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
MemoryCard::MemoryCard(QString library) throw (NoSuchLibraryException)
// -------------------------------------------------------------------------------------------------
        : m_library(library), m_portNumber(-1), m_cardTerminalNumber(-1)
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
void MemoryCard::init(int portNumber) throw (CardException)
// -------------------------------------------------------------------------------------------------
{
    if (m_cardTerminalNumber != -1 && m_portNumber != -1)
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
}


// -------------------------------------------------------------------------------------------------
void MemoryCard::close() throw (CardException, NotInitializedException)
// -------------------------------------------------------------------------------------------------
{
    if (m_cardTerminalNumber == -1 || m_portNumber == -1)
    {
        throw NotInitializedException("The MemoryCard object was closed althought it was "
            "not initalized.");
    }
    
    char ret = m_CT_close_function(m_cardTerminalNumber);
    if (ret != OK)
    {
        throw CardException(CardException::ErrorCode(ret));
    }
    m_portNumber = -1;
    m_cardTerminalNumber = -1;
}


// -------------------------------------------------------------------------------------------------
MemoryCard::~MemoryCard()
// -------------------------------------------------------------------------------------------------
{
    if (m_cardTerminalNumber != -1 && m_portNumber != -1)
    {
        qDebug("Closing in desctructor. Avoid this ...");
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
