/*
 * Id: $Id: qpamat.cpp,v 1.2 2003/10/20 20:56:30 bwalle Exp $
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
#include <qmainwindow.h>
#include <qaction.h>
#include <qkeysequence.h>
#include <qapplication.h>
#include <qpopupmenu.h>
#include <qtextstream.h>
#include <qnamespace.h>
#include <qmenubar.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qdockwindow.h>
#include <qtextedit.h>
#include <qdialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qscrollview.h>

#include "../images/new_16x16.xpm"
#include "../images/new_22x22.xpm"
#include "../images/exit_16x16.xpm"
#include "../images/exit_22x22.xpm"
#include "../images/save_16x16.xpm"
#include "../images/save_22x22.xpm"
#include "../images/whats_this.xpm"
#include "../images/lock_big.xpm"

#include "dialogs/passworddialog.h"
#include "dialogs/newpassworddialog.h"
#include "qpamat.h"
#include "settings.h"
#include "rightpanel.h"

// -------------------------------------------------------------------------------------------------
Qpamat::Qpamat()
// -------------------------------------------------------------------------------------------------
{
    // Title and Icon
    setIcon(lock_big_xpm);
    setCaption("QPaMaT");
    
    setUsesBigPixmaps(true);
    QIconSet::setIconSize(QIconSet::Small, QSize(16, 16));
    QIconSet::setIconSize(QIconSet::Large, QSize(22, 22));
    
    // Initialization of menu
    initActions();
    initMenubar();
    initToolbar();
    
    // Tree on the left
    QDockWindow* dock = new QDockWindow(this);
    dock->setVerticallyStretchable(true);
    dock->setResizeEnabled(true);
    dock->setCloseMode(QDockWindow::Never);
    dock->setCaption(tr("Tree"));
    dock->setFixedExtentWidth(qApp->desktop()->width() * 0.15);
    addDockWindow(dock, Qt::DockLeft);
    
    m_tree = new Tree(dock);
    dock->setWidget(m_tree);
    
    // main widget in the center
    m_rightPanel = new RightPanel(this);
    setCentralWidget(m_rightPanel);
    
    QObject::connect(m_tree, SIGNAL(selectionChanged(QListViewItem*)),
        m_rightPanel, SLOT(setItem(QListViewItem*)));
    
    // display statusbar
    statusBar();
    
    setLogin(false);
    
    // restore the layout
    QString layout = Settings::getInstance().getSettings().readEntry("Main Window/layout");
    QTextStream layoutStream(&layout, IO_ReadOnly);
    layoutStream >> *this;
    if (Settings::getInstance().getSettings().readBoolEntry("Main Window/maximized", false))
    {
        showMaximized();
    }
    else
    {
        resize(
        Settings::getInstance().getSettings().readNumEntry("Main Window/width", 
            qApp->desktop()->width() * 0.6 ),
        Settings::getInstance().getSettings().readNumEntry("Main Window/height", 
            qApp->desktop()->height() / 2 )
        );
    }
}


// -------------------------------------------------------------------------------------------------
void Qpamat::initToolbar()
// -------------------------------------------------------------------------------------------------
{
    // ----- Application ---------------------------------------------------------------------------
    QToolBar* applicationToolbar = new QToolBar(this);
    applicationToolbar->setLabel(tr("Application"));
    
    m_actions.newAction->addTo(applicationToolbar);
    m_actions.saveAction->addTo(applicationToolbar);
}


// -------------------------------------------------------------------------------------------------
void Qpamat::initMenubar()
// -------------------------------------------------------------------------------------------------
{
     // ----- File ---------------------------------------------------------------------------------
     QPopupMenu* fileMenu = new QPopupMenu(this);
     menuBar()->insertItem(tr("&File"), fileMenu);
     
     m_actions.newAction->addTo(fileMenu);
     m_actions.loginAction->addTo(fileMenu);
     m_actions.logoutAction->addTo(fileMenu);
     m_actions.saveAction->addTo(fileMenu);
     fileMenu->insertSeparator();
     m_actions.quitAction->addTo(fileMenu);
     
     // ----- Help ---------------------------------------------------------------------------------
     menuBar()->insertSeparator();
     QPopupMenu* helpMenu = new QPopupMenu(this);
     menuBar()->insertItem(tr("&Help"), helpMenu);
     
     m_actions.whatsThisAction->addTo(helpMenu);
     helpMenu->insertSeparator();
     m_actions.aboutQtAction->addTo(helpMenu);
     m_actions.aboutAction->addTo(helpMenu);
}


// -------------------------------------------------------------------------------------------------
void Qpamat::closeEvent(QCloseEvent* e)
// -------------------------------------------------------------------------------------------------
{
    if (m_loggedIn)
    {
        save();
    }
    
    // write window layout
    QString layout;
    QTextStream layoutStream(&layout, IO_WriteOnly);
    layoutStream << *this;
    Settings::getInstance().getSettings().writeEntry("Main Window/layout", layout);
    Settings::getInstance().getSettings().writeEntry("Main Window/width", size().width());
    Settings::getInstance().getSettings().writeEntry("Main Window/height", size().height());
    Settings::getInstance().getSettings().writeEntry("Main Window/maximized", isMaximized());
    
    e->accept();
}



// -------------------------------------------------------------------------------------------------
void Qpamat::login()
// -------------------------------------------------------------------------------------------------
{
    PasswordDialog* dlg = new PasswordDialog(this);
    bool ok = false;
    
    while (!ok)
    {
        
        if (dlg->exec() == QDialog::Accepted)
        {
            m_password = dlg->getPassword();
        }
        else
        {
            break;
        }
        
        // try to read the data
        try
        {
            m_tree->setEnabled(true);
            ok = m_tree->readFromXML( Settings::getInstance().getSettings().readEntry("/Data File",
                Settings::QPAMAT_FILE_NAME ), m_password);
        }
        catch (const WrongPassword& ex)
        {
            QMessageBox::warning(this, QObject::tr("QPaMaT"),
               tr("The passphrase you entered was wrong. Try again."),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        }
    }
    
    delete dlg;
    setLogin(ok);
}

// -------------------------------------------------------------------------------------------------
void Qpamat::setLogin(bool loggedIn)
// -------------------------------------------------------------------------------------------------
{
    m_loggedIn = loggedIn;
    
    m_actions.loginAction->setEnabled(!loggedIn);
    m_actions.logoutAction->setEnabled(loggedIn);
    m_actions.saveAction->setEnabled(loggedIn);
    if (!loggedIn)
    {
        m_tree->clear();
    }
    m_tree->setEnabled(loggedIn);
    
}


// -------------------------------------------------------------------------------------------------
void Qpamat::save()
// -------------------------------------------------------------------------------------------------
{
    if (m_loggedIn)
    {
        QSettings& settings = Settings::getInstance().getSettings();
        m_tree->writeToXML(settings.readEntry("/Data File", Settings::QPAMAT_FILE_NAME), m_password,
            settings.readEntry("/Cipher algorithm", Encryptor::getSuggestedAlgorithm()));
    }
}


// -------------------------------------------------------------------------------------------------
void Qpamat::logout()
// -------------------------------------------------------------------------------------------------
{
    save();
    setLogin(false);
}


// -------------------------------------------------------------------------------------------------
void Qpamat::newFile()
// -------------------------------------------------------------------------------------------------
{
    if (m_loggedIn)
    {
        logout();
    }
    
    NewPasswordDialog* dialog = new NewPasswordDialog(this);
    
    if (dialog->exec() == QDialog::Accepted)
    {
        m_password = dialog->getPassword();
        setLogin(true);
    }
    
    delete dialog;
}

// -------------------------------------------------------------------------------------------------
void Qpamat::initActions()
// -------------------------------------------------------------------------------------------------
{
    // ----- File ----------------------------------------------------------------------------------
    m_actions.newAction = new QAction(QIconSet(new_16x16_xpm, new_22x22_xpm), tr("&New"), 
        QKeySequence(CTRL|Key_N), this);
    connect(m_actions.newAction, SIGNAL(activated()), this, SLOT(newFile()));
    
    m_actions.quitAction = new QAction(QIconSet(exit_16x16_xpm, exit_22x22_xpm), tr("E&xit"),
        QKeySequence(CTRL|Key_Q), this);
    connect(m_actions.quitAction, SIGNAL(activated()), this, SLOT(close()));
    
    m_actions.loginAction = new QAction(tr("&Login"), QKeySequence(CTRL|Key_L), this);
    connect(m_actions.loginAction, SIGNAL(activated()), this, SLOT(login()));
    
    m_actions.logoutAction = new QAction(tr("&Logout"), QKeySequence(CTRL|Key_L), this);
    connect(m_actions.logoutAction, SIGNAL(activated()), this, SLOT(logout()));
    
    m_actions.saveAction = new QAction(QIconSet(save_16x16_xpm, save_22x22_xpm), tr("&Save"), 
        QKeySequence(CTRL|Key_S), this);
    connect(m_actions.saveAction, SIGNAL(activated()), this, SLOT(save()));
    
    // ----- Help ----------------------------------------------------------------------------------
    m_actions.whatsThisAction = new QAction(QPixmap(whats_this_xpm), tr("&What's this"), 
        QKeySequence(SHIFT|Key_F1), this);
    connect(m_actions.whatsThisAction, SIGNAL(activated()), this, SLOT(whatsThis()));
    
    m_actions.aboutAction = new QAction(tr("&About..."), QKeySequence(Key_F1), this);
    connect(m_actions.aboutAction, SIGNAL(activated()) , &m_help, SLOT(showAbout()));
    
    m_actions.aboutQtAction = new QAction(tr("About &Qt..."), QKeySequence(), this);
    connect(m_actions.aboutQtAction, SIGNAL(activated()), qApp, SLOT(aboutQt()));
    
}
    
