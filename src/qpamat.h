/*
 * Id: $Id: qpamat.h,v 1.12 2003/12/15 21:20:16 bwalle Exp $
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
#include <qtoolbutton.h>

#include "util/timerstatusmessage.h"
#include "help.h"
#include "tree.h"
#include "rightpanel.h"


/*!
 * \brief The main application window.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.12 $
 * \date $Date: 2003/12/15 21:20:16 $
 */
class Qpamat : public QMainWindow
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of the application.
         */
        Qpamat();
        
        /*!
         * Deletes the application.
         */
        ~Qpamat();
        
        /*!
         * Reimplemented to implement AutoLogin 
         */
        void show();
        
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
        
        /*!
         * Prints the current stuff.
         */
        void print();
        
        /*!
         * Sets the application state to "modified".
         */
        void setModified(bool modified = true);
    
        
    public slots:
        
        /*!
         * Prints a message in the statusbar.
         * \param message the message string
         * \param warning \c true if it's a warning, \c false if it's just a information
         *        Currently the time is determined by this argument.
         */
        void message(const QString& message, bool warning = TRUE);
        
    protected:
        
        /*!
         * Handles a closeEvent.
         */
        void closeEvent(QCloseEvent* evt);
        

    private:
        void initToolbar();
        void initMenubar();
        void initActions();
        void connectSignalsAndSlots();
        void setLogin(bool login);
        
    private:
        Tree* m_tree;
        QString m_password;
        Help m_help;
        QPopupMenu* m_treeContextMenu;
        TimerStatusmessage* m_message;
        RightPanel* m_rightPanel;
        QComboBox* m_searchCombo;
        QToolBar* m_searchToolbar;
        bool m_loggedIn;
        bool m_first;
        bool m_modified;
        
        struct ToolButtons
        {
            QToolButton* search;
        };
        ToolButtons m_toolButtons;
        struct Actions
        {
            QAction* newAction;
            QAction* loginAction;
            QAction* logoutAction;
            QAction* saveAction;
            QAction* viewTreeAction;
            QAction* quitAction;
            QAction* printAction;
            QAction* aboutAction;
            QAction* aboutQtAction;
            QAction* whatsThisAction;
            QAction* changePasswordAction;
            QAction* settingsAction;
            QAction* addItemAction;
            QAction* removeItemAction;
        };
        Actions m_actions;
};
#endif // QPAMAT_H
