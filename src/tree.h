/*
 * Id: $Id: tree.h,v 1.13 2003/12/29 15:12:27 bwalle Exp $
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

#include <qstring.h>
#include <qlistview.h>
#include <qpopupmenu.h>
#include <qdragobject.h>

#include "treeentry.h"
#include "security/encryptor.h"

class WrongPassword : public std::runtime_error
{
    public:
        WrongPassword(const std::string& error) : std::runtime_error(error) { }
};

// -------------------------------------------------------------------------------------------------

class Tree : public QListView
{
    Q_OBJECT
    
    public:
        enum MenuID
        {
            INSERT_ITEM, INSERT_CATEGORY, RENAME_ITEM, DELETE_ITEM
        };
        
    public:
        Tree(QWidget* parent);
        
        bool readFromXML(const QString& fileName, const QString& password) throw (WrongPassword);
        bool writeToXML(const QString& fileName, const QString& password, const QString& algorithm);
        
        QString toRichTextForPrint(); 
        
    public slots:
        void searchFor(const QString& word);
        void deleteCurrent();
        void insertAtCurrentPos();
        void setShowPasswordStrength(bool show );
        void updatePasswordStrengthView();
        void recomputePasswordStrength();
        
    signals:
        void selectionCleared();
        void stateModified();
        
    protected:
        QDragObject* dragObject();
        void keyPressEvent(QKeyEvent* evt);
        
    private slots:
        void showContextMenu(QListViewItem* item, const QPoint& point);
        void insertItem(TreeEntry* item, bool category = false);
        void currentChangedHandler(QListViewItem* item);
        void droppedHandler(QDropEvent* e);
        
    private:
        void initTreeContextMenu();
        void showCurruptedMessage(const QString& fileName);
        void showReadErrorMessage(const QString& message);
        bool writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber);
    
    private:
        QPopupMenu*  m_contextMenu;
        bool         m_showPasswordStrength;
};


#endif // TREE_H
