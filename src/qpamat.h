/*
 * Id: $Id: qpamat.h,v 1.5 2003/11/29 14:43:03 bwalle Exp $
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
#include <qcombobox.h>
#include <qevent.h>

#include "help.h"
#include "tree.h"
#include "rightpanel.h"


/*!
 * The main application window.
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.5 $
 * \date $Date: 2003/11/29 14:43:03 $
 */
class Qpamat : public QMainWindow
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of the application.
         */
        Qpamat();
        
    protected slots:
        
        /*!
         * Performs the login process.
         */
        void login();
        
        /*!
         * Performs the logout process.
         */
        void logout();
        
        /*!
         * Performs the newFile process.
         */
        void newFile();
        
        /*!
         * Performs the save process.
         */
        void save();
        
        /*!
         * Displayes the change password dialog.
         */
        void changePassword();
        
        /*! 
         * Displays the configuration dialog.
         */
        void configure();
        
        /*!
         * Searches in the tree for the specified word.
         */
        void search();
        
    
    protected:
        
        /*!
         * Handles a closeEvent.
         */
        void closeEvent(QCloseEvent* evt);

    private:
        void initToolbar();
        void initMenubar();
        void initActions();
        void setLogin(bool login);
        
    private:
        // the tree
        Tree* m_tree;
        QString m_password;
        Help m_help;
        QPopupMenu* m_treeContextMenu;
        RightPanel* m_rightPanel;
        QComboBox* m_searchCombo;
        QToolBar* m_searchToolbar;
        bool m_loggedIn;
        
        struct Actions
        {
            QAction* newAction;
            QAction* loginAction;
            QAction* logoutAction;
            QAction* saveAction;
            QAction* viewTreeAction;
            QAction* quitAction;
            QAction* aboutAction;
            QAction* aboutQtAction;
            QAction* whatsThisAction;
            QAction* changePasswordAction;
            QAction* settingsAction;
            QAction* searchAction;
        };
        Actions m_actions;
};
#endif // QPAMAT_H
