/*
 * Id: $Id: qpamat.cpp,v 1.1 2003/10/11 19:51:15 bwalle Exp $
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
#include <qnamespace.h>
#include <qmenubar.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qdockwindow.h>
#include <qtextedit.h>
#include <qdialog.h>

#include "../images/whats_this.xpm"
#include "../images/quit.xpm"
#include "../images/lock.xpm"
#include "../images/lock_big.xpm"

#include "dialogs/passworddialog.h"
#include "qpamat.h"
#include "globals.h"

// -------------------------------------------------------------------------------------------------
Qpamat::Qpamat()
// -------------------------------------------------------------------------------------------------
{
    // Initialization of menu
    initActions();
    initMenubar();
    
    // Title and Icon
    setIcon(lock_big_xpm);
    setCaption(QPAMAT_USERVISIBLE);
    
    // Tree on the left
    QDockWindow* dock = new QDockWindow(this);
    dock->setVerticallyStretchable(true);
    dock->setMovingEnabled(false);
    dock->setResizeEnabled(true);
    addDockWindow(dock, tr("Tree"), Qt::DockLeft);
    
    m_tree = new Tree(dock);
    dock->setWidget(m_tree);
    
    // main widget in the center
    setCentralWidget(new QTextEdit(this));
}


// -------------------------------------------------------------------------------------------------
void Qpamat::initMenubar()
// -------------------------------------------------------------------------------------------------
{
     // ----- File ---------------------------------------------------------------------------------
     QPopupMenu* fileMenu = new QPopupMenu(this);
     menuBar()->insertItem(tr("&File"), fileMenu);
     
     m_openAction->addTo(fileMenu);
     m_quitAction->addTo(fileMenu);
     
     // ----- Help ---------------------------------------------------------------------------------
     menuBar()->insertSeparator();
     QPopupMenu* helpMenu = new QPopupMenu(this);
     menuBar()->insertItem(tr("&Help"), helpMenu);
     
     m_whatsThisAction->addTo(helpMenu);
     helpMenu->insertSeparator();
     m_aboutQtAction->addTo(helpMenu);
     m_aboutAction->addTo(helpMenu);
}


// -------------------------------------------------------------------------------------------------
void Qpamat::closeEvent(QCloseEvent* e)
// -------------------------------------------------------------------------------------------------
{
    qDebug("Application exited");
    e->accept();
}

// -------------------------------------------------------------------------------------------------
void Qpamat::openFile()
// -------------------------------------------------------------------------------------------------
{
    PasswordDialog* dlg = new PasswordDialog(this);
    
    if (dlg->exec() == QDialog::Accepted)
    {
        qDebug("Accepted, %s\n", static_cast<const char*>(dlg->getPassword()));
    }
    else
    {
        qDebug("Rejected");
    }
    
    delete dlg;
}

// -------------------------------------------------------------------------------------------------
void Qpamat::initActions()
// -------------------------------------------------------------------------------------------------
{
    // ----- File ----------------------------------------------------------------------------------
    m_quitAction = new QAction(QPixmap(quit_xpm), tr("E&xit"),
        QKeySequence(CTRL|Key_Q), this);
    connect(m_quitAction, SIGNAL(activated()), this, SLOT(close()));
    
    m_openAction = new QAction(tr("&Open"), QKeySequence(CTRL|Key_O), this);
    connect(m_openAction, SIGNAL(activated()), this, SLOT(openFile()));
    
    // ----- Help ----------------------------------------------------------------------------------
    m_whatsThisAction = new QAction(QPixmap(whats_this_xpm), tr("&What's this"), 
        QKeySequence(SHIFT|Key_F1), this);
    connect(m_whatsThisAction, SIGNAL(activated()), this, SLOT(whatsThis()));
    
    m_aboutAction = new QAction(tr("&About..."), QKeySequence(Key_F1), this);
    connect( m_aboutAction, SIGNAL(activated()) , &m_help, SLOT(showAbout()));
    
    m_aboutQtAction = new QAction(tr("About &Qt..."), QKeySequence(), this);
    connect(m_aboutQtAction, SIGNAL(activated()), qApp, SLOT(aboutQt()));
}
    
