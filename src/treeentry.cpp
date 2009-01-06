/*
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
#include <QString>
#include <QDomDocument>
#include <QTextStream>
#include <QDropEvent>

#include "qpamat.h"

#include "treeentry.h"
#include "settings.h"
#include "tree.h"


/*!
    \class TreeEntry

    \brief Represents an entry in the tree.

    \ingroup gui
    \author Bernhard Walle
*/

/*!
    \typedef TreeEntry::PropertyIterator

    The iterator for the proeprties
*/

/*!
    \fn TreeEntry::propertyAppended()

    Fired is a property was added.
*/

/*!
    Returns the weakest passwort strength of any children of the item. All password
    strength should be computed because of speed issues (in other words, no wait cursor
    or something else is displayed in this function).
    \return the password strength, Property::PUndefined should be never returned
    \exception PasswordCheckException if recomputing is necessary and the PasswordChecker
               threw a PasswordCheckException
*/
Property::PasswordStrength TreeEntry::weakestChildrenPassword() const throw (PasswordCheckException)
{
    Property::PasswordStrength lowest = Property::PUndefined;

    if (m_isCategory) {
        TreeEntry* item = dynamic_cast<TreeEntry*>(firstChild());
        while (item) {
            Property::PasswordStrength strength = item->weakestChildrenPassword();
            if (strength < lowest) {
                lowest = strength;
                if (lowest == Property::PWeak)
                    break;
            }
            item = dynamic_cast<TreeEntry*>(item->nextSibling());
        }
    } else {
        bool hadPasswords = false;
        PropertyIterator it = propertyIterator();
        Property* current;
        while ( (current = it.current()) != 0 ) {
            ++it;
            if (current->getType() == Property::PASSWORD) {
                hadPasswords = true;
                Property::PasswordStrength strength = current->getPasswordStrength();
                if (strength < lowest) {
                    lowest = strength;
                    if (lowest == Property::PWeak)
                        break;
                }
            }
        }
    }

    return lowest;
}


/*!
    Returns the name of the entry.
*/
QString TreeEntry::getName() const
{
    return m_name;
}


/*!
    Returns whether the entry is a categroy.
*/
bool TreeEntry::isCategory() const
{
    return m_isCategory;
}


/*!
    Returns an iterator to the first entry of the properties list.
    \return the iterator
*/
Property* TreeEntry::getProperty(uint index)
{
    Q_ASSERT( index < m_properties.count() );
    return m_properties.at(index);
}


/*!
    Returns the name of the entry.
    \param column the column
    \return the name
    \sa QListViewItem::text
*/
QString TreeEntry::text(int column) const
{
    Q_ASSERT( column == 0 );
    column++; // no warnings
    return m_name;
}


/*!
    Sets the text of the entry. Sets the name internally.
    \param column the column
    \param text the new text
*/
void TreeEntry::setText(int column, const QString& text)
{
    UNUSED(column);
    Q_ASSERT(column == 0);

    m_name = text;
    listView()->sort();
    listView()->triggerUpdate();
}


/*!
    Moves the given property one step up.
    \param index the index of the property
*/
void TreeEntry::movePropertyOneUp(uint index)
{
    Q_ASSERT( index < m_properties.count() - 1);

    m_properties.setAutoDelete(false);
    Property* h = m_properties.at(index+1);
    m_properties.insert(index, h);
    m_properties.remove(index+2);
    m_properties.setAutoDelete(true);
}


/*!
    Moves the given property one step down.
    \param index the index of the property
*/
void TreeEntry::movePropertyOneDown(uint index)
{
    Q_ASSERT( index > 0 && index < m_properties.count() );

    m_properties.setAutoDelete(false);
    Property* h = m_properties.at(index);
    m_properties.insert(index-1, h);
    m_properties.remove(index+1);
    m_properties.setAutoDelete(true);
}


/*!
    Deletes the property with the specified index
    \param index the index
*/
void TreeEntry::deleteProperty(uint index)
{
    Q_ASSERT( index < m_properties.count() );
    m_properties.remove(index);
}


/*!
    Removes all properties.
*/
void TreeEntry::deleteAllProperties()
{
    m_properties.clear();
}


/*!
    Inserts a new property at the end.
    \param property the property
*/
void TreeEntry::appendProperty(Property* property)
{
    m_properties.append(property);
    emit propertyAppended();
}


/*!
    Returns an iterator for the list
*/
TreeEntry::PropertyIterator TreeEntry::propertyIterator() const
{
    return PropertyIterator(m_properties);
}


/*!
    This function converts a tree entry to HTML for printing. A TreeEntry represents one
    big table.
    \return the RichText
*/
QString TreeEntry::toRichTextForPrint() const
{
    if (m_isCategory)
        return QString("");

    QString catString;
    const Q3ListViewItem* item = this;
    while ((item = item->parent()))
        catString = catString.prepend( dynamic_cast<const TreeEntry*>(item)->getName() + ": ");

    QString ret;
    ret += QString("<table width=\"100%\"><tr><td bgcolor=grey cellpadding=\"3\">"
            "&nbsp;<b>%1</b></td></tr><tr><td>"
            "<table border=\"0\">").arg(catString + m_name);

    PropertyIterator it = propertyIterator();
    Property* current;
    while ( (current = it.current()) != 0 ) {
        ++it;
        ret += current->toRichTextForPrint();
    }

    ret += "</table></td></tr></table<p>&nbsp;<p>";
    return ret;
}


/*!
    Appends the tree entry as text representation to the given stream. The text is formatted for
    export.

    \param stream the stream where the text is appended
*/
void TreeEntry::appendTextForExport(QTextStream& stream)
{
    if (m_isCategory)
        return;

    QString catString;
    const Q3ListViewItem* item = this;
    while ((item = item->parent()))
        catString = catString.prepend( dynamic_cast<const TreeEntry*>(item)->getName() + ": ");

    stream << "--------------------------------------------------------------------------------\n";
    stream << catString + m_name << "\n";
    stream << "--------------------------------------------------------------------------------\n";
    stream << "\n";

    PropertyIterator it = propertyIterator();
    Property* current;
    while ((current = it.current()) != 0) {
        ++it;
        current->appendTextForExport(stream);
    }

    stream << "\n\n";
}


/*!
    Appends the treeentry as \c category or \c entry tag in the XML structure.
    \param document the document needed to create new elements
    \param parent the parent to which the new created element should be attached
*/
void TreeEntry::appendXML(QDomDocument& document, QDomNode& parent) const
{
    QDomElement newElement;
    if (m_isCategory) {
        newElement = document.createElement("category");
        newElement.setAttribute("wasOpen", isOpen());
        TreeEntry* child = dynamic_cast<TreeEntry*>(firstChild());

        while(child) {
            child->appendXML(document, newElement);
            child = dynamic_cast<TreeEntry*>(child->nextSibling());
        }
    } else {
        newElement = document.createElement("entry");

        PropertyIterator it(m_properties);
        Property* property;
        while ( (property = it.current()) != 0 ) {
            ++it;
            property->appendXML(document, newElement);
        }
    }
    newElement.setAttribute("name", m_name);
    newElement.setAttribute("isSelected", isSelected());
    parent.appendChild(newElement);
}


/*!
    Converts this TreeEntry to XML. This XML is used for drag and drop. It contains one
    \<entry\> or \<category\> tag and this tag contains also an attribute named
    \c memoryAddress which holds the memory address for this item. This can be used
    for deleting the object or for determine whether the user is dragging to iself.
    \return the XML string
*/
QString TreeEntry::toXML() const
{
    QDomDocument doc;
    appendXML(doc, doc);
    doc.documentElement().setAttribute(QString("memoryAddress"), (qlonglong)this);

    return doc.toString();
}


/*!
    Checks if the item can accept drops of the type QMimeSource. The MIME type accepted is
    <tt>application/x-qpamat</tt>. Only categories accept drops.
    \param mime the QMimeSource object
    \return \c true if the item can accept drops of type QMimeSource mime; otherwise
            \c false.
*/
bool TreeEntry::acceptDrop(const QMimeSource* mime) const
{
    return mime->provides("application/x-qpamat");
}


/*!
    Overwritten drop handler
    \param evt the event
*/
void TreeEntry::dropped(QDropEvent *evt)
{
    if (evt->provides("application/x-qpamat")) {
        evt->accept();
        QString xml = QString::fromUtf8(evt->encodedData("application/x-qpamat"));
        QDomDocument doc;
        doc.setContent(xml);
        QDomElement elem = doc.documentElement();

        Q3ListViewItem* src = reinterpret_cast<TreeEntry*>(elem.attribute("memoryAddress").toLong());

        if (src == this) {
            qpamat->message(tr("Cannot dray to itself."));
            return;
        }

        TreeEntry* item = m_isCategory ? this : dynamic_cast<TreeEntry*>(parent());
        TreeEntry* appended = 0;
        if (item)
            appended = appendFromXML(item, elem);
        else
            appended = appendFromXML(listView(), elem);

        if (!isOpen())
            setOpen(true);

        listView()->setSelected(appended, true);
        dynamic_cast<Tree*>(listView())->updatePasswordStrengthView();
        delete src;
    }
}

// :maxLineLen=100:
