/*
 * Id: $Id: tree.h,v 1.6 2003/11/28 18:42:43 bwalle Exp $
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

#include "wrongpassword.h"
#include "treeentry.h"
#include "security/nosuchalgorithmexception.h"
#include "security/encryptor.h"

/**
 * Represents the tree that holds the password entries.
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.6 $
 * @date $Date: 2003/11/28 18:42:43 $
 */
class Tree : public QListView
{
    Q_OBJECT
    
    public:
        
        /** Holds the menu ids */
        enum MenuID
        {
            INSERT_ITEM,
            INSERT_CATEGORY,
            RENAME_ITEM,
            DELETE_ITEM
        };
        /**
         * Creates a new instance of a Tree.
         * @param parent the parent
         */
        Tree(QWidget* parent = 0);
        
        /**
         * Reads and updates the tree from the given XML file.
         * @param fileName the XML file from which the information should be read
         * @param password the password
         * @return \c true on success, \c false  otherwise
         * @exception WrongPassword if the given password was wrong according to the XML file
         */
        bool readFromXML(const QString& fileName, const QString& password) throw (WrongPassword);
        
        
        /**
         * Writs the current tree to the XML file. The error handling is done by the function,
         * i.e. suitable dialog messages are displayed.
         * @param fileName the file name where the tree should be written
         * @param password the password for encryption
         * @param algorithm the algorithm for encryption
         */
        void writeToXML(const QString& fileName, const QString& password, const QString& algorithm);
        
        /**
         * If the users presses a key.
         * @param evt the key event
         */
        void keyPressEvent(QKeyEvent* evt);
    
    public slots:
        
        /**
         * Searches in the tree for the specified word.
         * @param word the word which should be found
         */
        void searchFor(const QString& word);
        
    private slots:
        void showContextMenu(QListViewItem* item, const QPoint& point);
        void insertItem(bool category, TreeEntry* item);
        
    private:
        void initTreeContextMenu();
        void showCurruptedMessage(const QString& fileName);
    
    private:
        QPopupMenu* m_contextMenu; 
};


#endif // TREE_H
