/*
 * Id: $Id: qpamat.h,v 1.1 2003/10/11 19:51:15 bwalle Exp $
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
#ifndef QPAMAT_H
#define QPAMAT_H

#include <qmainwindow.h>
#include <qaction.h>
#include <qevent.h>

#include "help.h"
#include "tree.h"

/**
 * The main application window.
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/10/11 19:51:15 $
 */
class Qpamat : public QMainWindow
{
    Q_OBJECT
    
    public:
        
        /**
         * Constans for menus and menu entries.
         */
        enum MenuID {
            // menus 
            FILE_MENU, HELP_MENU,
            
            // entries in the file menu
            NEW_FILE,
            
            // entries in the help menu
            ABOUT_QT, ABOUT, WHATS_THIS
        };
        
        Qpamat();
        
    protected slots:
        void openFile();
    
    protected:
        void closeEvent(QCloseEvent* evt);

    private:
        void initMenubar();
        void initActions();
        
    private:
        // the tree
        Tree* m_tree;
        
        // objects
        Help m_help;
        
        // actions
        QAction* m_openAction;
        QAction* m_quitAction;
        QAction* m_aboutAction;
        QAction* m_aboutQtAction;
        QAction* m_whatsThisAction;
};
#endif // QPAMAT_H
