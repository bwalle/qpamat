/*
 * Id: $Id: qpamat.cpp,v 1.10 2003/12/06 18:22:49 bwalle Exp $
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
#include <iostream>

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
#include <qlabel.h>
#include <qsettings.h>
#include <qscrollview.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qfont.h>
#include <qcursor.h>
#include <qeventloop.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h> 
#include <qdatetime.h>

#include "../images/new_16x16.xpm"
#include "../images/new_22x22.xpm"
#include "../images/exit_16x16.xpm"
#include "../images/exit_22x22.xpm"
#include "../images/save_16x16.xpm"
#include "../images/save_22x22.xpm"
#include "../images/whats_this.xpm"
#include "../images/lock_big.xpm"
#include "../images/configure_16x16.xpm"
#include "../images/configure_22x22.xpm"
#include "../images/find_16x16.xpm"
#include "../images/find_22x22.xpm"
#include "../images/print_16x16.xpm"
#include "../images/print_22x22.xpm"

#include "dialogs/passworddialog.h"
#include "dialogs/newpassworddialog.h"
#include "dialogs/configurationdialog.h"
#include "security/externalpasswordchecker.h"
#include "security/symmetricencryptor.h"
#include "qpamat.h"
#include "settings.h"
#include "rightpanel.h"

// -------------------------------------------------------------------------------------------------
Qpamat::Qpamat()
// -------------------------------------------------------------------------------------------------
        : m_first(true)
{
    // Title and Icon
    setIcon(lock_big_xpm);
    setCaption("QPaMaT");
    
    setUsesBigPixmaps(true);
    QIconSet::setIconSize(QIconSet::Small, QSize(16, 16));
    QIconSet::setIconSize(QIconSet::Large, QSize(22, 22));
    
    // Tree on the left
    QDockWindow* dock = new QDockWindow(this);
    dock->setVerticallyStretchable(true);
    dock->setResizeEnabled(true);
    dock->setCloseMode(QDockWindow::Always);
    dock->setCaption(tr("Sites"));
    dock->setFixedExtentWidth(int(qApp->desktop()->width() * 0.15));
    addDockWindow(dock, Qt::DockLeft);
    
    m_tree = new Tree(dock);
    dock->setWidget(m_tree);
    
    // Initialization of menu
    initActions();
    initMenubar();
    initToolbar();
    
    // main widget in the center
    m_rightPanel = new RightPanel(this);
    setCentralWidget(m_rightPanel);
    
    QObject::connect(m_tree, SIGNAL(selectionChanged(QListViewItem*)),
        m_rightPanel, SLOT(setItem(QListViewItem*)));
    connect(m_tree, SIGNAL(selectionCleared()), m_rightPanel, SLOT(clear()));
    
    // display statusbar
    statusBar();
    setLogin(false);
    
    // restore settings
    QSettings& set = Settings::getInstance().getSettings();
    
    // restore history
    QStringList list = QStringList::split (" | ", set.readEntry("Main Window/SearchHistory") );
    m_searchCombo->insertStringList(list);
    m_searchCombo->clearEdit();
    
    // restore the layout
    QString layout = set.readEntry("Main Window/layout");
    QTextStream layoutStream(&layout, IO_ReadOnly);
    layoutStream >> *this;
    if (set.readBoolEntry("Main Window/maximized", false))
    {
        showMaximized();
    }
    else
    {
        resize(
            set.readNumEntry("Main Window/width", int(qApp->desktop()->width() * 0.6) ),
            set.readNumEntry("Main Window/height", int(qApp->desktop()->height() / 2.0) )
        );
    }
}

// -------------------------------------------------------------------------------------------------
void Qpamat::show()
// -------------------------------------------------------------------------------------------------
{    
    QWidget::show();
    if (m_first)
    {
        if (Settings::getInstance().getSettings().readBoolEntry("General/AutoLogin", false))
        {
            login();
        }
        m_first = false;
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
    m_actions.printAction->addTo(applicationToolbar);
    m_actions.settingsAction->addTo(applicationToolbar);
    
    // ----- Search --------------------------------------------------------------------------------
    m_searchToolbar = new QToolBar(this);
    m_searchToolbar->setLabel(tr("Search"));
    
    new QLabel(tr("Search:"), m_searchToolbar);
    m_searchCombo = new QComboBox(true, m_searchToolbar);
    m_searchCombo->setMinimumWidth(120);
    m_searchCombo->setDuplicatesEnabled(false);
    m_searchCombo->setSizeLimit(20);
    
    m_actions.searchAction->addTo(m_searchToolbar);
    
    connect(m_searchCombo, SIGNAL(activated(int)), this, SLOT(search()));
    
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
     m_actions.printAction->addTo(fileMenu);
     fileMenu->insertSeparator();
     m_actions.quitAction->addTo(fileMenu);
     
     // ----- View ---------------------------------------------------------------------------------
     menuBar()->insertItem(tr("&View"), createDockWindowMenu(QMainWindow::NoToolBars));
     
     // ----- Options ------------------------------------------------------------------------------
     QPopupMenu* optionsMenu = new QPopupMenu(this);
     menuBar()->insertItem(tr("&Options"), optionsMenu);
     
     m_actions.changePasswordAction->addTo(optionsMenu);
     m_actions.settingsAction->addTo(optionsMenu);
     
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
    
    QSettings& set = Settings::getInstance().getSettings();
    
    // write the history
    QStringList list;
    for (int i = 0; i < m_searchCombo->count(); ++i)
    {
        list.append(m_searchCombo->text(i));
    }
    set.writeEntry("Main Window/SearchHistory", list.join(" | "));
    
    // write window layout
    QString layout;
    QTextStream layoutStream(&layout, IO_WriteOnly);
    layoutStream << *this;
    set.writeEntry("Main Window/layout", layout);
    set.writeEntry("Main Window/width", size().width());
    set.writeEntry("Main Window/height", size().height());
    set.writeEntry("Main Window/maximized", isMaximized());
    
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
            ok = m_tree->readFromXML( Settings::getInstance().getSettings().readEntry(
                "/General/Datafile", Settings::QPAMAT_FILE_NAME ), m_password);
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
    m_actions.changePasswordAction->setEnabled(loggedIn);
    m_actions.printAction->setEnabled(loggedIn);
    m_searchToolbar->setEnabled(loggedIn);
    
    if (!loggedIn)
    {
        m_tree->clear();
        m_rightPanel->clear();
        this->setFocus();
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
        m_tree->writeToXML(settings.readEntry("/General/Datafile", Settings::QPAMAT_FILE_NAME), 
            m_password, settings.readEntry("/Security/CipherAlgorithm",
                SymmetricEncryptor::getSuggestedAlgorithm()));
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
void Qpamat::changePassword()
// -------------------------------------------------------------------------------------------------
{
    NewPasswordDialog* dlg = new NewPasswordDialog(this, m_password);
    if (dlg->exec() == QDialog::Accepted)
    {
        m_password = dlg->getPassword();
    }
    delete dlg;
}


// -------------------------------------------------------------------------------------------------
void Qpamat::configure()
// -------------------------------------------------------------------------------------------------
{
    ConfigurationDialog* dlg = new ConfigurationDialog(this);
    dlg->exec();
    delete dlg;
}


// -------------------------------------------------------------------------------------------------
void Qpamat::search()
// -------------------------------------------------------------------------------------------------
{
    const QString text = m_searchCombo->currentText();
    
    if (text.length() == 0)
    {
         QMessageBox::warning(this, QObject::tr("QPaMaT"),
               tr("Please enter a search criterion in the text field!"),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
    else
    {
        m_tree->searchFor(text);
    }
}

// -------------------------------------------------------------------------------------------------
void Qpamat::print()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings(); 
    QPrinter printer( QPrinter::HighResolution );
    printer.setFullPage( TRUE );
    if ( printer.setup( this ) ) 
    {
        QPainter p;
        if ( !p.begin( &printer ) )
        {
            return;
        }
        
        QFont serifFont;
        QFont sansSerifFont;
        serifFont.fromString(set.readEntry( "Printing/NormalFont", Settings::DEFAULT_NORMAL_FONT));
        sansSerifFont.fromString(set.readEntry("Printing/FooterFont",Settings::DEFAULT_FOOTER_FONT));
        
        p.setFont(sansSerifFont);
        
        qApp->setOverrideCursor( QCursor( Qt::WaitCursor ) );
        qApp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput );
        
        QPaintDeviceMetrics metrics(p.device());
        int dpiy = metrics.logicalDpiY();
#define CON_MM(x)( int( ( (x)/25.4)*dpiy ) )
        int margin = CON_MM(20);
        QRect body( margin, margin, metrics.width() - 2 * margin, 
            metrics.height() - 2 * margin - CON_MM(8) );
        QSimpleRichText richText(m_tree->toRichTextForPrint(), serifFont, QString::null, 0, 
                QMimeSourceFactory::defaultFactory(), body.height(), Qt::black, false );
        richText.setWidth( &p, body.width() );
        QRect view( body );
        QString programString = tr("QPaMaT - Password managing tool for Unix, Windows and MacOS X");
        QString dateString = QDate::currentDate().toString(Qt::ISODate) + " / " +
            QTime::currentTime().toString("hh:mm");
        
        for (int page = 1; ; ++page)
        {
            qApp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput );
            QString pageS = tr("page") + " " + QString::number(page);
            
            richText.draw( &p, body.left(), body.top(), view, colorGroup() );
            view.moveBy(0, body.height() );
            p.translate(0 , -body.height() );
            
            int x_pos = int(( view.left() + p.fontMetrics().width(programString)
                + view.right() - p.fontMetrics().width( pageS ) ) / 2.0
                - p.fontMetrics().width( dateString ) / 2.0 );
            int y_pos = view.bottom() + p.fontMetrics().ascent() + CON_MM(3);
            p.drawLine(view.left(), view.bottom()+CON_MM(3), view.right(), view.bottom()+CON_MM(3));
            p.drawText( view.left(), y_pos, programString);
            p.drawText( x_pos, y_pos, dateString );
            p.drawText( view.right() - p.fontMetrics().width(pageS), y_pos, pageS);
            if ( view.top() >= richText.height() )
            {
                break;
            }
            printer.newPage();
        }
#undef CONVERT_MM
        
        qApp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput );
        qApp->restoreOverrideCursor();
    }
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
    
    m_actions.printAction = new QAction(QIconSet(print_16x16_xpm, print_22x22_xpm),
        tr("&Print..."), QKeySequence(CTRL|Key_P), this);
    connect(m_actions.printAction, SIGNAL(activated()), this, SLOT(print()));
    
    m_actions.saveAction = new QAction(QIconSet(save_16x16_xpm, save_22x22_xpm), tr("&Save"), 
        QKeySequence(CTRL|Key_S), this);
    connect(m_actions.saveAction, SIGNAL(activated()), this, SLOT(save()));
    
    // ----- Options -------------------------------------------------------------------------------
    m_actions.changePasswordAction = new QAction(tr("&Change Password..."), QKeySequence(), this);
    connect(m_actions.changePasswordAction, SIGNAL(activated()), this, SLOT(changePassword()));
    
    m_actions.settingsAction = new QAction(QIconSet(configure_16x16_xpm, configure_22x22_xpm), 
        tr("&Settings..."), QKeySequence(CTRL|Key_C), this);
    connect(m_actions.settingsAction, SIGNAL(activated()), this, SLOT(configure()));
    
    // ----- Help ----------------------------------------------------------------------------------
    m_actions.whatsThisAction = new QAction(QPixmap(whats_this_xpm), tr("&What's this"), 
        QKeySequence(SHIFT|Key_F1), this);
    connect(m_actions.whatsThisAction, SIGNAL(activated()), this, SLOT(whatsThis()));
    
    m_actions.aboutAction = new QAction(tr("&About..."), QKeySequence(Key_F1), this);
    connect(m_actions.aboutAction, SIGNAL(activated()) , &m_help, SLOT(showAbout()));
    
    m_actions.aboutQtAction = new QAction(tr("About &Qt..."), QKeySequence(), this);
    connect(m_actions.aboutQtAction, SIGNAL(activated()), qApp, SLOT(aboutQt()));
    
    // ----- Toolbar -------------------------------------------------------------------------------
    m_actions.searchAction = new QAction(QIconSet(find_16x16_xpm, find_22x22_xpm), 
        tr("&Search"), QKeySequence(), this);
    connect(m_actions.searchAction, SIGNAL(activated()), this, SLOT(search()));
    
}
    
