/*
 * Id: $Id: qpamat.h,v 1.22 2004/12/26 17:44:38 bwalle Exp $
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
#include <qcombobox.h>
#include <qevent.h>
#include <qtoolbutton.h>
#include <qaction.h>

#include "settings.h"
#include "randompassword.h"
#include "help.h"

// forward declarations
class Tree;
class RightPanel;
class TimerStatusmessage;

class Qpamat : public QMainWindow
{
    Q_OBJECT
    
    public:
        Qpamat();
        ~Qpamat();
        
        Settings& set();
        
    protected slots:
        void login();
        bool logout();
        void newFile();
        void save();
        void changePassword();
        void configure();
        void search();
        void print();
        void clearClipboard();
        void setModified(bool modified = true);
        void passwordStrengthHandler(bool enabled);
        void exportData();
        bool exportOrSave();
        
    signals:
        void insertPassword(const QString& password);
        void settingsChanged();
        
    public slots:
        void message(const QString& message, bool warning = TRUE);
        
    protected:
        void closeEvent(QCloseEvent* evt);
        
    private:
        void initToolbar();
        void initMenubar();
        void initActions();
        void connectSignalsAndSlots();
        void setLogin(bool login);
        
    private:
        struct Actions
        {
            QAction* newAction;
            QAction* loginAction;
            QAction* logoutAction;
            QAction* saveAction;
            QAction* exportAction;
            QAction* viewTreeAction;
            QAction* quitAction;
            QAction* printAction;
            QAction* helpAction;
            QAction* aboutAction;
            QAction* aboutQtAction;
            QAction* whatsThisAction;
            QAction* changePasswordAction;
            QAction* settingsAction;
            QAction* addItemAction;
            QAction* removeItemAction;
            QAction* randomPasswordAction;
            QAction* passwordStrengthAction;
            QAction* clearClipboardAction;
            QAction* focusSearch;
        };
        struct ToolButtons
        {
            QToolButton* search;
        };
        
    private:
        Settings            m_settings;
        Tree*               m_tree;
        QString             m_password;
        Help                m_help;
        QPopupMenu*         m_treeContextMenu;
        TimerStatusmessage* m_message;
        RightPanel*         m_rightPanel;
        QComboBox*          m_searchCombo;
        QToolBar*           m_searchToolbar;
        RandomPassword*     m_randomPassword;
        bool                m_loggedIn;
        bool                m_modified;
        ToolButtons         m_toolButtons;
        Actions             m_actions;
    
    private:
        Qpamat(const Qpamat&);
        Qpamat& operator=(const Qpamat&);
};


extern Qpamat* qpamat;


#endif // QPAMAT_H
