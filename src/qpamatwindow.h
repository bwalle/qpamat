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
#ifndef QPAMATWINDOW_H
#define QPAMATWINDOW_H

#include <QMainWindow>
#include <Q3MainWindow>
#include <QComboBox>
#include <QEvent>
#include <QToolButton>
#include <QAction>
#include <Q3PopupMenu>
#include <QCloseEvent>
#include <QLabel>
#include <QSystemTrayIcon>

#include "settings.h"
#include "randompassword.h"
#include "help.h"

// forward declarations
class Tree;
class RightPanel;
class TimerStatusmessage;

class QpamatWindow : public QMainWindow
{
    Q_OBJECT

    friend class QpamatAdaptor;

    public:
        QpamatWindow();
        ~QpamatWindow();

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
        void handleTrayiconClick();
        void handleTrayiconClick(QSystemTrayIcon::ActivationReason reason);
        void exitHandler();

    signals:
        void insertPassword(const QString& password);
        void settingsChanged();
        void quit();

    public slots:
        void message(const QString& message, bool warning = TRUE);

    protected:
        void closeEvent(QCloseEvent* evt);
        void initToolbar();
        void initMenubar();
        void initActions();
        void connectSignalsAndSlots();
        void setLogin(bool login);

    private:
        struct Actions
        {
            QAction* newAction;
            QAction* loginLogoutAction;
            QAction* saveAction;
            QAction* exportAction;
            QAction* viewTreeAction;
            QAction* quitAction;
            QAction* quitActionNoKeyboardShortcut;
            QAction* searchAction;
            QAction* showHideAction;
            QAction* printAction;
            QAction* helpAction;
            QAction* aboutAction;
            QAction* aboutQtAction;
            QAction* changePasswordAction;
            QAction* settingsAction;
            QAction* addItemAction;
            QAction* removeItemAction;
            QAction* randomPasswordAction;
            QAction* passwordStrengthAction;
            QAction* clearClipboardAction;
            QAction* focusSearch;
        };

    private:
        QLabel*             m_searchLabel;
        Settings            m_settings;
        Tree*               m_tree;
        QString             m_password;
        Help                m_help;
        Q3PopupMenu*         m_treeContextMenu;
        TimerStatusmessage* m_message;
        RightPanel*         m_rightPanel;
        QComboBox*          m_searchCombo;
        RandomPassword*     m_randomPassword;
        bool                m_loggedIn;
        bool                m_modified;
        Actions             m_actions;
        QSystemTrayIcon*    m_trayIcon;
        QRect               m_lastGeometry;

    private:
        QpamatWindow(const QpamatWindow&);
        QpamatWindow& operator=(const QpamatWindow&);
};


extern QpamatWindow* qpamatwindow;


#endif // QPAMATWINDOW_H

// vim: set sw=4 ts=4 et:
