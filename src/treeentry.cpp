/*
 * Id: $Id: treeentry.cpp,v 1.2 2003/10/11 19:50:52 bwalle Exp $
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

#include "treeentry.h"


// -------------------------------------------------------------------------------------------------
TreeEntry::~TreeEntry()
// -------------------------------------------------------------------------------------------------
{
    // children are deleted automatically
    // vector elements are deleted automatically by the vector
}

// -------------------------------------------------------------------------------------------------
QString TreeEntry::getName() const
// -------------------------------------------------------------------------------------------------
{
    return m_name;
}


// -------------------------------------------------------------------------------------------------
bool TreeEntry::isCategory() const
// -------------------------------------------------------------------------------------------------
{
    return m_isCategory;
}


// -------------------------------------------------------------------------------------------------
Property* TreeEntry::getProperty(uint index)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE
    Q_ASSERT( index < m_properties.count() );
#endif
    return m_properties.at(index);
}


// -------------------------------------------------------------------------------------------------
QString TreeEntry::text(int column) const
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE 
    Q_ASSERT( column == 0 );
#endif
    column++; // no warnings
    return m_name;
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::setText(int column, const QString& text)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE
    Q_ASSERT(column == 0);
#endif
    m_name = text;
    listView()->sort();
    listView()->triggerUpdate();
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::movePropertyOneUp(uint index)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE 
    Q_ASSERT( index > 0 && index < m_properties.count() );
#endif
    m_properties.setAutoDelete(false);
    Property* h = m_properties.at(index);
    m_properties.insert(index, m_properties.at(index-1));
    m_properties.insert(index-1, h);
    m_properties.setAutoDelete(true);
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::movePropertyOneDown(uint index)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE 
    Q_ASSERT( index < m_properties.count()-1 );
#endif
    m_properties.setAutoDelete(false);
    Property* h = m_properties.at(index);
    m_properties.insert(index, m_properties.at(index+1));
    m_properties.insert(index+1, h);
    m_properties.setAutoDelete(true);
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::deleteProperty(uint index)
// -------------------------------------------------------------------------------------------------
{
#ifdef QT_CHECK_RANGE
    Q_ASSERT( index < m_properties.count() );
#endif
    m_properties.remove(index);
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::deleteAllProperties()
// -------------------------------------------------------------------------------------------------
{
    m_properties.clear();
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::appendProperty(Property* property)
// -------------------------------------------------------------------------------------------------
{
    m_properties.append(property);
}


// -------------------------------------------------------------------------------------------------
TreeEntry::PropertyIterator TreeEntry::propertyIterator()
// -------------------------------------------------------------------------------------------------
{
    return PropertyIterator(m_properties);
}


// -------------------------------------------------------------------------------------------------
void TreeEntry::appendXML(QDomDocument& document, QDomNode& parent, const Encryptor& enc) const
// -------------------------------------------------------------------------------------------------
{
    QDomElement newElement;
    if (m_isCategory)
    {
        newElement = document.createElement("category");
        newElement.setAttribute("wasOpen", isOpen());
        TreeEntry* child = dynamic_cast<TreeEntry*>(firstChild());
        while(child) 
        {
            child->appendXML(document, newElement, enc);
            child = dynamic_cast<TreeEntry*>(nextSibling());
        }
    }
    else
    {
        newElement = document.createElement("entry");
        
        PropertyIterator it(m_properties);
        Property* property;
        while ( (property = it.current()) != 0 )
        {
            ++it;
            property->appendXML(document, newElement, enc);
        }
    }
    newElement.setAttribute("name", m_name);
    newElement.setAttribute("isSelected", isSelected());
    parent.appendChild(newElement);
}
