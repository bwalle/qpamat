/*
 * Id: $Id: tree.h,v 1.11 2003/12/13 22:33:44 bwalle Exp $
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

#include "wrongpassword.h"
#include "treeentry.h"
#include "security/nosuchalgorithmexception.h"
#include "security/encryptor.h"

/*!
 * \brief Represents the tree that holds the password entries.
 *
 * This class also handles reading and writing to XML files (by calling the right methods
 * of an TreeEntry) and -- very important -- writing to the smartcard. Each XML file has a
 * random number which is created on each successful write. It is used to identify the card.
 *
 * So the first byte (a value between 0 and 255) is the random number. Then two bytes on the
 * card indicate the number of bytes stored. Then a fill byte is stored which is reserved for future
 * use. The 5th byte on the card is then the first real byte for passwords.
 
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.11 $
 * \date $Date: 2003/12/13 22:33:44 $
 */
class Tree : public QListView
{
    Q_OBJECT
    
    public:
        
        /*! Holds the menu ids */
        enum MenuID
        {
            INSERT_ITEM,
            INSERT_CATEGORY,
            RENAME_ITEM,
            DELETE_ITEM
        };
        /*!
         * Creates a new instance of a Tree.
         * \param parent the parent
         */
        Tree(QWidget* parent = 0);
        
        /*!
         * Reads and updates the tree from the given XML file.
         * \param fileName the XML file from which the information should be read
         * \param password the password
         * \return \c true on success, \c false  otherwise
         * \exception WrongPassword if the given password was wrong according to the XML file
         */
        bool readFromXML(const QString& fileName, const QString& password) throw (WrongPassword);
        
        
        /*!
         * Writs the current tree to the XML file. The error handling is done by the function,
         * i.e. suitable dialog messages are displayed.
         * \param fileName the file name where the tree should be written
         * \param password the password for encryption
         * \param algorithm the algorithm for encryption
         * \return \c true on success, \c false  otherwise
         */
        bool writeToXML(const QString& fileName, const QString& password, const QString& algorithm);
        
        /*!
         * If the users presses a key.
         * \param evt the key event
         */
        void keyPressEvent(QKeyEvent* evt);
        
        /*!
         * Converts all entries to RichText for printing.
         * \return the HTML string
         */
        QString toRichTextForPrint(); 
        
    signals:
        
        /*!
         * If the tree has a current item but no item is selected. 
         */
        void selectionCleared();
        
        /*!
         *
         * If something was modified, need to determine if saving is necessary.
         */
        void stateModified();
        
    protected:
        
        /*!
         * Returns the appropriage QDragObject for performing Drag and drop
         * \return the object
         */
        QDragObject* dragObject();
    
    public slots:
        
        /*!
         * Searches in the tree for the specified word.
         * \param word the word which should be found
         */
        void searchFor(const QString& word);
        
        /*!
         * Deletes the current item if the widget has the focus.
         */
        void deleteCurrent();
        
        /*!
         * Inserts an item at the current position if the widget has the focus.
         */
        void insertAtCurrentPos();
        
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
        QPopupMenu* m_contextMenu; 
};


#endif // TREE_H
