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
#ifndef TREE_H
#define TREE_H

#include <stdexcept>

#include <QString>
#include <Q3ListView>
#include <Q3PopupMenu>
#include <Q3DragObject>
#include <QTextStream>
#include <QKeyEvent>
#include <QDropEvent>

#include "treeentry.h"
#include "security/encryptor.h"

class Tree : public Q3ListView
{
    Q_OBJECT
    
    public:
        enum MenuID
        {
            INSERT_ITEM, INSERT_CATEGORY, RENAME_ITEM, DELETE_ITEM
        };
        
    public:
        Tree(QWidget* parent);
        
        void readFromXML(const QDomElement& document);
        void appendXML(QDomDocument& doc) const 
            throw (std::invalid_argument);
        
        QString toRichTextForPrint();
        void appendTextForExport(QTextStream& stream);
        
    public slots:
        void searchFor(const QString& word);
        void deleteCurrent();
        void insertAtCurrentPos();
        void setShowPasswordStrength(bool show );
        void updatePasswordStrengthView();
        void recomputePasswordStrength(bool* error = 0);
        
    signals:
        void selectionCleared();
        void stateModified();
        
    protected:
        Q3DragObject* dragObject();
        void keyPressEvent(QKeyEvent* evt);
        
    private slots:
        void showContextMenu(Q3ListViewItem* item, const QPoint& point);
        void insertItem(TreeEntry* item, bool category = false);
        void currentChangedHandler(Q3ListViewItem* item);
        void droppedHandler(QDropEvent* e);
        
    private:
        void initTreeContextMenu();
        void showReadErrorMessage(const QString& message);
        bool writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber);
    
    private:
        Q3PopupMenu*  m_contextMenu;
        bool         m_showPasswordStrength;
};


#endif // TREE_H
