/*
 * Id: $Id$
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
#ifndef TREEENTRY_H
#define TREEENTRY_H

#include <QString>
#include <Q3PtrList>
#include <Q3ListView>
#include <QEvent>
#include <QTextStream>
#include <QDropEvent>
#include <Q3ValueList>

#include "property.h"

typedef Q3PtrList<Property> PropertyPtrList;

class TreeEntry : public QObject, public Q3ListViewItem
{
    Q_OBJECT
    
    using Q3ListViewItem::parent;
    
    public:
        typedef Q3PtrListIterator<Property> PropertyIterator;
        
    public:
        template<class T>
        TreeEntry(T* parent, const QString& name = QString::null, bool isCategory = false);
        
        QString getName() const;
        bool isCategory() const;
        
        Property* getProperty(uint index);
        void appendProperty(Property* property);
        PropertyIterator propertyIterator() const;
        
        Property::PasswordStrength weakestChildrenPassword() const throw (PasswordCheckException);
        
        void appendXML(QDomDocument& document, QDomNode& parent) const;
        
        QString text(int column) const;
        void setText(int column, const QString& text);
        
        QString toRichTextForPrint() const;
        void appendTextForExport(QTextStream& stream);
        QString toXML() const;
        
        bool acceptDrop(const QMimeSource* mime) const;
        
    public:
        template<class T>
        static TreeEntry* appendFromXML(T* parent, QDomElement& element);
        
    public slots:
        void movePropertyOneUp(uint index);
        void movePropertyOneDown(uint index);
        void deleteProperty(uint index);
        void deleteAllProperties();
        
    signals:
        void propertyAppended();
        
    protected:
        void dropped(QDropEvent *evt);
        
    private:
        QString             m_name;
        PropertyPtrList     m_properties;
        bool                m_isCategory;
        bool                m_weak;
        
    private:
        void init();
    
    private:
        TreeEntry(const TreeEntry&);
        TreeEntry& operator=(const TreeEntry&);
};

#include "treeentry.ipp"

#endif // TREEENTRY_H

// :maxLineLen=100:
