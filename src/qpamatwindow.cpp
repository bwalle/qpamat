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
#include <QDebug>
#include <QApplication>
#include <QTextDocument>
#include <QAction>
#include <QShortcut>
#include <QKeySequence>
#include <QMenu>
#include <QTextStream>
#include <QToolBar>
#include <QMenuBar>
#include <QIcon>
#include <QDesktopWidget>
#include <QPixmap>
#include <QAbstractTextDocumentLayout>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QSettings>
#include <QDockWidget>
#include <QPrinter>
#include <QPainter>
#include <QClipboard>
#include <QFont>
#include <QEventLoop>
#include <QCursor>
#include <QFileDialog>
#include <QCloseEvent>
#include <QScopedPointer>

#include "qpamatwindow.h"

#include "settings.h"
#include "datareadwriter.h"
#include "timerstatusmessage.h"
#include "dialogs/passworddialog.h"
#include "dialogs/newpassworddialog.h"
#include "dialogs/configurationdialog.h"
#include "util/timeoutapplication.h"
#include "util/platformhelpers.h"
#include "rightpanel.h"
#include "tree.h"

#ifdef Q_WS_WIN
#  define TRAY_ICON_FILE_NAME ":/images/qpamat_16.png"
#else
#  define TRAY_ICON_FILE_NAME ":/images/qpamat_34.png"
#endif

#define CON_MM(x)( int( ( (x)/25.4)*dpiy ) )

/**
 * @class QpamatWindow
 *
 * @brief This is the main class of the QPaMaT application.
 *
 * This class represents the main window, so it initializes actions, menubars
 * and toolbars, creates the Tree and the RightPanel, loads and saves setting.
 * It handles some actions like printing directly by calling the right
 * functions from the other classes and delegates some actions to other
 * classes.
 *
 * It makes no sense to have more than one instances of this class. There's a
 * global pointer \c qpamat which holds the address of the qpamat. This
 * pointer is initialized \em after the full initialization of QpamatWindow,
 * i.e. don't use it in constructors of Tree and RightPanel because theses are
 * generated by QpamatWindow and therefore \c qpamatwindow is not initialized
 * (it's just \c 0).
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @fn QpamatWindow::insertPassword(const QString&)
 *
 * This signal is emitted if a password was generated for inserting.
 */

/**
 * @fn QpamatWindow::settingsChanged()
 *
 * This signals is emitted if the settings have changed.
 */


/**
 * @fn QpamatWindow::quit()
 *
 * If the application should quit.
 */

// -------------------------------------------------------------------------------------------------

/**
 * @brief Creates a new instance of the application.
 */
QpamatWindow::QpamatWindow()
    : QMainWindow(0, "qpamat main window")
    , m_tree(0)
    , m_treeContextMenu(0)
    , m_message(0)
    , m_rightPanel(0)
    , m_searchCombo(0)
    , m_randomPassword(0)
    , m_trayIcon(0)
    , m_lastGeometry(0, 0, 0, 0)
{
    QRect geometry;

    // Title and Icon
    setIcon(QPixmap(":/images/qpamat_48.png"));
    setCaption("QPaMaT");

    setIconSize(QSize(24, 24));

    // Random password, we need this for the tree
    m_randomPassword = new RandomPassword(this, "Random Password");

    // retrieve the geometry only from the current screen
    geometry = qApp->desktop()->screenGeometry(this);

    // Tree on the left
    QDockWidget* dock = new QDockWidget(tr("Sites"), this);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock->setObjectName("Sites");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setMinimumWidth(int(geometry.width() * 0.15));

    m_tree = new Tree(dock);
    dock->setWidget(m_tree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    // main widget in the center
    m_rightPanel = new RightPanel(this);
    setCentralWidget(m_rightPanel);

    // Initialization of menu
    initActions();
    initMenubar();
    initToolbar();

    // display statusbar
    statusBar();
    m_message.reset(new TimerStatusmessage(statusBar()));
    setLogin(false);

    // restore history
    QStringList list = QStringList::split (" | ", set().readEntry("Main Window/SearchHistory") );
    m_searchCombo->insertStringList(list);
    m_searchCombo->clearEdit();

    // restore the layout
    restoreState(set().readByteArrayEntry("Main Window/layout"));
    if (set().readBoolEntry("Main Window/maximized"))
        showMaximized();
    else
        resize(
            set().readNumEntry("Main Window/width", int(geometry.width() * 0.6) ),
            set().readNumEntry("Main Window/height", int(geometry.height() / 2.0) )
        );

    QString rightpanel = set().readEntry("Main Window/rightpanelLayout");
    if (!rightpanel.isNull()) {
        QTextStream rightpanelStream(&rightpanel, QIODevice::ReadOnly);
        rightpanelStream >> *m_rightPanel;
    }

    // tray icon
    if (set().readBoolEntry("Presentation/SystemTrayIcon") &&
            QSystemTrayIcon::isSystemTrayAvailable()) {
        QMenu* trayPopup = new QMenu(this);
        trayPopup->addAction(m_actions.showHideAction);
        trayPopup->addAction(m_actions.quitActionNoKeyboardShortcut);

        m_trayIcon = new QSystemTrayIcon(QPixmap(TRAY_ICON_FILE_NAME), this);
        m_trayIcon->setToolTip(tr("QPaMaT"));
        m_trayIcon->setContextMenu(trayPopup);
        m_trayIcon->show();

        // hack to prevent icontray events to interfere with the timeout mechanism
        dynamic_cast<TimeoutApplication*>(qApp)->addReceiverToIgnore(m_trayIcon);

        connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                SLOT(handleTrayiconClick(QSystemTrayIcon::ActivationReason)));
    }

    connectSignalsAndSlots();

    if (set().readBoolEntry("General/AutoLogin")) {
        if (QFile::exists(set().readEntry("General/Datafile")) )
            QTimer::singleShot( 0, this, SLOT(login()) );
        else
            QTimer::singleShot( 0, this, SLOT(newFile()) );
    }

    setUnifiedTitleAndToolBarOnMac(true);

    // don't show icons in the menu bar by default since that seems to be rather seldom
    // on the Mac platform
    if (RUNNING_ON_MAC) {
        qApp->setAttribute(Qt::AA_DontShowIconsInMenus);
    }
}


/**
 * @brief Deletes the application.
 */
QpamatWindow::~QpamatWindow()
{}


/**
 * @brief Returns the settings object.
 *
 * @return a reference to the object
 */
Settings& QpamatWindow::set()
{
    return m_settings;
}


/**
 * @brief Prints a message in the statusbar.
 *
 * @param message the message string
 * @param warning \c true if it's a warning, \c false if it's just a information
 *                Currently the time is determined by this argument.
 */
void QpamatWindow::message(const QString& message, bool)
{
    m_message->message(message, /*warning ? 3000 : */ 1500);
}


/**
 * @brief Initializes the toolbar.
 */
void QpamatWindow::initToolbar()
{
    // ----- Application ---------------------------------------------------------------------------
    QToolBar* applicationToolbar = new QToolBar(tr("Application"), this);
    applicationToolbar->setObjectName("Application");
    addToolBar(applicationToolbar);

    applicationToolbar->addAction(m_actions.newAction);
    applicationToolbar->addAction(m_actions.saveAction);
    applicationToolbar->addAction(m_actions.loginLogoutAction);
    applicationToolbar->addAction(m_actions.printAction);
    applicationToolbar->addAction(m_actions.settingsAction);

    // ----- Search --------------------------------------------------------------------------------
    QToolBar* searchToolbar = new QToolBar(tr("Search"), this);
    searchToolbar->setObjectName("Search");
    addToolBar(searchToolbar);
    m_searchLabel = new QLabel(tr("Search:")+" ", searchToolbar);

    m_searchCombo = new QComboBox(true, searchToolbar);
    m_searchCombo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
    m_searchCombo->setObjectName("SearchCombo");
    m_searchCombo->setDuplicatesEnabled(false);
    m_searchCombo->setFocusPolicy(Qt::ClickFocus);
    m_searchCombo->setInsertionPolicy(QComboBox::AtTop);
    m_searchCombo->setAutoCompletion(true);

    searchToolbar->addWidget(m_searchLabel);
    searchToolbar->addWidget(m_searchCombo);
    searchToolbar->addAction(m_actions.searchAction);

    // ---- Edit toolbar ---------------------------------------------------------------------------
    QToolBar* editToolbar = new QToolBar(tr("Edit"), this);
    editToolbar->setObjectName("Edit");
    addToolBar(editToolbar);

    editToolbar->addAction(m_actions.addItemAction);
    editToolbar->addAction(m_actions.removeItemAction);

    searchToolbar->adjustSize();
    editToolbar->adjustSize();
}


/**
 * @brief Initializes the menubar.
 */
void QpamatWindow::initMenubar()
{
     // ----- File ---------------------------------------------------------------------------------
     QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(m_actions.newAction);
     fileMenu->addAction(m_actions.loginLogoutAction);
     fileMenu->addAction(m_actions.saveAction);
     fileMenu->addAction(m_actions.exportAction);
     fileMenu->addAction(m_actions.printAction);
     fileMenu->insertSeparator();
     fileMenu->addAction(m_actions.quitAction);

     // ----- Options ------------------------------------------------------------------------------
     QMenu* optionsMenu = menuBar()->addMenu(tr("&Options"));
     optionsMenu->addAction(m_actions.changePasswordAction);
     optionsMenu->addAction(m_actions.settingsAction);

     // ----- Extras -------------------------------------------------------------------------------
     QMenu* extrasMenu = menuBar()->addMenu(tr("&Extras"));
     extrasMenu->addAction(m_actions.randomPasswordAction);
     extrasMenu->addAction(m_actions.passwordStrengthAction);
     extrasMenu->addAction(m_actions.clearClipboardAction);

     // ----- Help ---------------------------------------------------------------------------------

     // align the "Help" menu on the right in the Motif and CDE style
     menuBar()->insertSeparator();
     QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(m_actions.helpAction);
     helpMenu->insertSeparator();
     helpMenu->addAction(m_actions.aboutQtAction);
     helpMenu->addAction(m_actions.aboutAction);
}


/**
 * @brief Saves the data if the application is closes.
 */
void QpamatWindow::exitHandler()
{
    if (!logout())
        return;

    // write the history
    QStringList list;
    int max = QMIN(m_searchCombo->count(), 10);
    for (int i = 0; i < max; ++i)
        list.append(m_searchCombo->text(i));
    set().writeEntry("Main Window/SearchHistory", list.join(" | "));

    // write window layout
    QString rightpanelLayout;
    QTextStream rightpanelStream(&rightpanelLayout, QIODevice::WriteOnly);
    rightpanelStream << *m_rightPanel;
    set().writeEntry("Main Window/layout", saveState());
    set().writeEntry("Main Window/width", size().width());
    set().writeEntry("Main Window/height", size().height());
    set().writeEntry("Main Window/maximized", isMaximized());
    set().writeEntry("Main Window/rightpanelLayout", rightpanelLayout);

    emit quit();
}


/**
 * @brief Handles a closeEvent of the main window.
 *
 * If the tray icon is visible, the application is just hidden. If no tray
 * icon is used, the application is exited.
 *
 * @param e the QCloseEvent object
 */
void QpamatWindow::closeEvent(QCloseEvent* e)
{
    if (m_trayIcon) {
        showHideWindow();
        e->ignore();
    } else {
        exitHandler();
        e->accept();
        QWidget::closeEvent(e);
    }
}


/**
 * @brief Sets the document status to modified or not modified.
 *
 * @param modified \c true if the document is modified, \c false if it is not modified
 */
void QpamatWindow::setModified(bool modified)
{
    m_modified = modified;
    m_actions.saveAction->setEnabled(modified);
}

/**
 * @brief Toggles the show status of the main window
 *
 * If the main window is shown, hide it. If it's not shown, show it.
 */
void QpamatWindow::showHideWindow()
{
    if (isShown()) {
        m_lastGeometry = geometry();
        hide();
        m_actions.showHideAction->setMenuText(tr("&Show"));
    } else {
        if (!(m_lastGeometry.x() == 0 && m_lastGeometry.y() == 0
                    && m_lastGeometry.width() == 0 && m_lastGeometry.height() == 0)) {
            setGeometry(m_lastGeometry);
        }
        show();
        m_actions.showHideAction->setMenuText(tr("&Hide"));
    }
}

/**
 * @brief Handles single click on the tray icon.
 *
 * Calls showHideWindow() if necessary (left click, but not on MacOS)dfriedhofdd.
 */
void QpamatWindow::handleTrayiconClick(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger && !RUNNING_ON_MAC) {
        showHideWindow();
    }
}


/**
 * @brief Does the login.
 */
void QpamatWindow::login()
{
    qDebug() << CURRENT_FUNCTION << "Calling login()";

    QScopedPointer<PasswordDialog> dlg(new PasswordDialog(this));
    QDomDocument doc;
    bool ok = false;

    while (!ok) {
        if (dlg->exec() == QDialog::Accepted)
            m_password = dlg->getPassword();
        else
            return;

        DataReadWriter reader(this);
        while (!ok) {
            try {
                doc = reader.readXML(m_password);
                ok = true;
            } catch (const ReadWriteException& e) {
                // type of the message
                QMessageBox::Icon icon = e.getSeverity() == ReadWriteException::WARNING
                    ? QMessageBox::Warning
                    : QMessageBox::Critical;
                // category: different behaviour
                ReadWriteException::Category cat = e.getCategory();

                if (cat == ReadWriteException::CAbort)
                    return;

                bool retry = e.retryMakesSense();

                QMessageBox *mb = new QMessageBox( "QPaMaT", e.getMessage(), icon,
                    (retry ? QMessageBox::Retry : QMessageBox::Ok) | QMessageBox::Default,
                    (retry ? QMessageBox::Abort : QMessageBox::NoButton),
                    QMessageBox::NoButton, this, "qt_msgbox_information", true,
                        Qt::WDestructiveClose);
                if (mb->exec() != QMessageBox::Retry)
                    return;

                // the user wants to continue, now we discriminate if we just retry
                // or show the password dialog again
                if (cat == ReadWriteException::CWrongPassword)
                    break;
                else
                    continue;
            }
        }
    }

    m_tree->readFromXML(doc.documentElement().namedItem("passwords").toElement());

    setLogin(true);
}


/**
 * @brief Helping function for setting the login status
 *
 * @param loggedIn \c true if the user has logged in, \c false if he has logged out
 */
void QpamatWindow::setLogin(bool loggedIn)
{
    qDebug() << CURRENT_FUNCTION << "Caling setLogin =" << loggedIn;
    m_loggedIn = loggedIn;

    // toggle action
    disconnect(m_actions.loginLogoutAction, SIGNAL(activated()), 0, 0 );
    if (loggedIn) {
        m_actions.loginLogoutAction->setIcon(createIcon("logout"));
        m_actions.loginLogoutAction->setMenuText(tr("&Logout"));
        m_actions.loginLogoutAction->setToolTip(tr("Logout"));
        connect(m_actions.loginLogoutAction, SIGNAL(activated()), SLOT(logout()));
    } else {
        m_actions.loginLogoutAction->setIconSet(createIcon("login"));
        m_actions.loginLogoutAction->setMenuText(tr("&Login"));
        m_actions.loginLogoutAction->setToolTip(tr("Login"));
        connect(m_actions.loginLogoutAction, SIGNAL(activated()), SLOT(login()));
    }

    m_actions.saveAction->setEnabled(loggedIn);
    m_actions.changePasswordAction->setEnabled(loggedIn);
    m_actions.printAction->setEnabled(loggedIn);
    m_searchCombo->setEnabled(loggedIn);
    m_actions.searchAction->setEnabled(loggedIn);
    m_searchLabel->setEnabled(loggedIn);
    m_actions.addItemAction->setEnabled(loggedIn);
    m_actions.removeItemAction->setEnabled(loggedIn);
    m_actions.passwordStrengthAction->setEnabled(loggedIn);
    m_actions.exportAction->setEnabled(loggedIn);

    if (loggedIn) {
        dynamic_cast<TimeoutApplication*>(qApp)->setTimeout(
            set().readNumEntry("Security/AutoLogout")
        );
    } else {
        m_actions.passwordStrengthAction->setOn(false);
        m_tree->clear();
        m_rightPanel->clear();
        this->setFocus();
    }

    m_tree->setEnabled(loggedIn);
    setModified(false);
}


/**
 * @brief Performs the save operation.
 */
void QpamatWindow::save()
{
    if (m_loggedIn && exportOrSave()) {
        setModified(false);
        message(tr("Wrote data successfully."));
    }
}


/**
 * @brief Exports or saves the data.
 *
 * The file name is determined by the global property General/Datafile and the
 * property Smartcard/UseCard is considered for smartcard writing.
 *
 * @return \c true if the action was successful, \c false otherwise
 */
bool QpamatWindow::exportOrSave()
{
    DataReadWriter writer(this);
    QDomDocument doc = writer.createSkeletonDocument();
    m_tree->appendXML(doc);
    bool success = false;
    while (!success) {
        try {
            writer.writeXML(doc, m_password);
            success = true;
        } catch (const ReadWriteException& e) {
            QMessageBox::Icon icon = e.getSeverity() == ReadWriteException::WARNING
                ? QMessageBox::Warning
                : QMessageBox::Critical;
            bool retry = e.retryMakesSense();
            QMessageBox *mb = new QMessageBox( "QPaMaT", e.getMessage(), icon,
                (retry ? QMessageBox::Retry : QMessageBox::Ok) | QMessageBox::Default,
                (retry ? QMessageBox::Abort : QMessageBox::NoButton),
                QMessageBox::NoButton, this, "qt_msgbox_information", true, Qt::WDestructiveClose);

            // ask the user if the exception type is not "abort"
            if (e.getCategory() == ReadWriteException::CAbort
                    || mb->exec() != QMessageBox::Retry) {
                break;
            }
        }
    }
    return success;
}


/**
 * @brief Exports the data.
 */
void QpamatWindow::exportData()
{
    QString oldFilename = set().readEntry("General/Datafile");
    QString fileName;

    QFileDialog* fd = new QFileDialog(this, tr("QPaMaT"), QDir::homeDirPath(),
        tr("QPaMaT XML files (*.xml);;Text files with cleartext password (*.txt)"));
    fd->setMode(QFileDialog::AnyFile);
    bool oldCard = set().readBoolEntry("Smartcard/UseCard");

    if (fd->exec() == QDialog::Accepted)
        fileName = fd->selectedFile();
    else
        return;

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("QPaMaT"),
            tr("The file you've choosen exists. Do you overwrite it?"),
            tr("&Overwrite"), tr("&Don't export"), QString::null, 1, 1) == 1)
            return;
    }

    // XML or text?
    if (fd->selectedFilter().endsWith("(*.xml)")) {
        set().writeEntry("General/Datafile", fileName);
        set().writeEntry("Smartcard/UseCard", false);
        if (m_loggedIn && exportOrSave())
            message(tr("Wrote data successfully."));
        set().writeEntry("General/Datafile", oldFilename);
        set().writeEntry("Smartcard/UseCard", oldCard);
    } else {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            m_tree->appendTextForExport(stream);
            file.close();
            message(tr("Wrote data successfully."));
        } else
            QMessageBox::warning(this, tr("QPaMaT"),
               tr("An error occured while saving the file."),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
}


/**
 * @brief Performs the logout operation.
 *
 * @return \c true if logout should be continued, \c false otherwise
 */
bool QpamatWindow::logout()
{
    // save the data
    if (m_modified) {
        qDebug() << CURRENT_FUNCTION << "Disable timeout action temporary";
        dynamic_cast<TimeoutApplication*>(qApp)->setTemporaryDisabled(true);

        int ret = QMessageBox::question(this, "QPaMaT", tr("There is modified data that was not saved."
            "\nDo you want to save it now?"), QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No, QMessageBox::Cancel) ;

        switch (ret) {
            case QMessageBox::Yes:
                save();
                break;

            case QMessageBox::Cancel:
                return false;

            default:
                break;
        }

        qDebug() << CURRENT_FUNCTION << "Enable timeout action again";
        dynamic_cast<TimeoutApplication*>(qApp)->setTemporaryDisabled(false);
    }

    setLogin(false);

    return true;
}


/**
 * @brief Creates a new file.
 */
void QpamatWindow::newFile()
{
    QScopedPointer<NewPasswordDialog> dialog(new NewPasswordDialog(this));

    if (dialog->exec() == QDialog::Accepted) {
        if (m_loggedIn)
            logout();

        m_password = dialog->getPassword();
        setLogin(true);
        setModified();
    }
}


/**
 * @brief Changes the password of the current session.
 */
void QpamatWindow::changePassword()
{
    QScopedPointer<NewPasswordDialog> dlg(new NewPasswordDialog(this, m_password));
    if (dlg->exec() == QDialog::Accepted) {
        m_password = dlg->getPassword();
        setModified();
    }
}


/**
 * @brief Displays the configuration dialog.
 */
void QpamatWindow::configure()
{
    QScopedPointer<ConfigurationDialog> dlg(new ConfigurationDialog(this));
    if (dlg->exec() == QDialog::Accepted)
        emit settingsChanged();
}


/**
 * @brief Does the search.
 */
void QpamatWindow::search()
{
    const QString text = m_searchCombo->currentText();

    if (text.length() == 0)
         message(tr("Please enter a search criterion in the text field!"));
    else
        m_tree->searchFor(text);
}


/**
 * @brief Handles the passowrd strength toggle action.
 *
 * @param enabled \c true if the password strength should be displayed, \c false otherwise
 */
void QpamatWindow::passwordStrengthHandler(bool enabled)
{
    m_tree->setShowPasswordStrength(enabled);
    bool error = false;
    if (enabled) {
        m_tree->recomputePasswordStrength(&error);
        if (error)
            m_actions.passwordStrengthAction->setOn(false);
    }

    if (!error) {
        m_tree->updatePasswordStrengthView();
    }
}


/**
 * @brief Prints the current document.
 *
 * Displayes the print dialog for that reason.
 */
void QpamatWindow::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage( TRUE );
    if ( printer.setup( this ) ) {
        QPainter p(&printer);
        // Check that there is a valid device to print to.
        if (!p.device())
            return;

        QFont serifFont;
        QFont sansSerifFont;
        serifFont.fromString(set().readEntry("Presentation/NormalFont"));
        sansSerifFont.fromString(set().readEntry("Presentation/FooterFont"));
        p.setFont(sansSerifFont);

        qApp->setOverrideCursor( QCursor( Qt::WaitCursor ) );
        qApp->processEvents( QEventLoop::ExcludeUserInput );

        const int dpiy = p.device()->logicalDpiY();
        const int margin = (int) ((2/2.54)*dpiy); // 2 cm margins

        QRectF body(margin, margin, p.device()->width() - 2*margin, p.device()->height() - 2*margin);

        QTextDocument* doc = new QTextDocument();
        doc->setHtml(m_tree->toRichTextForPrint());
        doc->setDefaultFont(serifFont);
        QAbstractTextDocumentLayout* layout = doc->documentLayout();
        layout->setPaintDevice(&printer);
        doc->setPageSize(body.size());

        QRectF view(0, 0, body.width(), body.height());
        p.translate(body.left(), body.top());

        QString programString = tr("QPaMaT - Password managing tool for Unix, Windows and MacOS X");
        QString dateString = QDate::currentDate().toString(Qt::ISODate) + " / " +
            QTime::currentTime().toString("hh:mm");

        int page = 1;
        do {
            qApp->processEvents( QEventLoop::ExcludeUserInput );
            QString pageS = tr("page") + " " + QString::number(page);

            QAbstractTextDocumentLayout::PaintContext ctx;
            p.setClipRect(view);
            ctx.clip = view;
            layout->draw(&p, ctx);

            p.setClipping(false);
            p.setFont(sansSerifFont);

            int x_pos = qRound(( view.left() + p.fontMetrics().width(programString)
                + view.right() - p.fontMetrics().width( pageS ) ) / 2.0
                - p.fontMetrics().width( dateString ) / 2.0 );
            int y_pos = qRound(view.bottom() + p.fontMetrics().ascent() + CON_MM(3));
            p.drawLine(qRound(view.left()), qRound(view.bottom()+CON_MM(3)),
                qRound(view.right()), qRound(view.bottom()+CON_MM(3)));
            p.drawText( qRound(view.left()), y_pos, programString);
            p.drawText( x_pos, y_pos, dateString );
            p.drawText( qRound(view.right() - p.fontMetrics().width(pageS)), y_pos, pageS);

            view.translate(0, body.height());
            p.translate(0 , -body.height());

            if (view.top() >= layout->documentSize().height())
                break;

            printer.newPage();
            page++;
        }
        while (true);
        Q_ASSERT(page == doc->pageCount());

        delete doc;

        qApp->processEvents( QEventLoop::ExcludeUserInput );
        qApp->restoreOverrideCursor();
    }
}


/**
 * @brief Clears the clipboard.
 */
void QpamatWindow::clearClipboard()
{
    QClipboard *cb = QApplication::clipboard();
    cb->setText(QString::null, QClipboard::Clipboard);
    cb->setText(QString::null, QClipboard::Selection);
}


/**
 * @brief Connects all signals with slots.
 */
void QpamatWindow::connectSignalsAndSlots()
{
    connect(m_tree, SIGNAL(selectionChanged(Q3ListViewItem*)),
        m_rightPanel, SLOT(setItem(Q3ListViewItem*)));
    connect(m_tree, SIGNAL(selectionCleared()), m_rightPanel, SLOT(clear()));


    // Actions
    connect(m_actions.newAction, SIGNAL(activated()), this, SLOT(newFile()));
    connect(m_actions.quitAction, SIGNAL(activated()), this, SLOT(exitHandler()));
    connect(m_actions.quitActionNoKeyboardShortcut, SIGNAL(activated()), this, SLOT(exitHandler()));
    connect(m_actions.showHideAction, SIGNAL(activated()), this, SLOT(showHideWindow()));
    connect(m_actions.printAction, SIGNAL(activated()), this, SLOT(print()));
    connect(m_actions.saveAction, SIGNAL(activated()), this, SLOT(save()));
    connect(m_actions.exportAction, SIGNAL(activated()), this, SLOT(exportData()));

    connect(m_actions.changePasswordAction, SIGNAL(activated()), this, SLOT(changePassword()));
    connect(m_actions.settingsAction, SIGNAL(activated()), this, SLOT(configure()));

    connect(m_actions.helpAction, SIGNAL(activated()), &m_help, SLOT(showHelp()));
    connect(m_actions.aboutAction, SIGNAL(activated()) , &m_help, SLOT(showAbout()));
    connect(m_actions.aboutQtAction, SIGNAL(activated()), qApp, SLOT(aboutQt()));

    connect(m_actions.randomPasswordAction, SIGNAL(activated()),
        m_randomPassword, SLOT(requestPassword()));
    connect(m_actions.clearClipboardAction, SIGNAL(activated()), SLOT(clearClipboard()));

    // password strength
    connect(m_actions.passwordStrengthAction, SIGNAL(toggled(bool)), SLOT(passwordStrengthHandler(bool)));
    connect(this, SIGNAL(settingsChanged()), m_tree, SLOT(recomputePasswordStrength()));
    connect(m_rightPanel, SIGNAL(passwordStrengthUpdated()), m_tree, SLOT(updatePasswordStrengthView()));

    // edit toolbar
    connect(m_actions.addItemAction, SIGNAL(activated()), m_tree, SLOT(insertAtCurrentPos()));
    connect(m_actions.addItemAction, SIGNAL(activated()), m_rightPanel, SLOT(insertAtCurrentPos()));
    connect(m_actions.removeItemAction, SIGNAL(activated()), m_tree, SLOT(deleteCurrent()));
    connect(m_actions.removeItemAction, SIGNAL(activated()), m_rightPanel, SLOT(deleteCurrent()));

    // search function
    connect(m_searchCombo, SIGNAL(activated(int)), this, SLOT(search()));
    connect(m_actions.searchAction, SIGNAL(activated()), this, SLOT(search()));

    // modified
    connect(m_tree, SIGNAL(stateModified()), SLOT(setModified()));
    connect(m_rightPanel, SIGNAL(stateModified()), SLOT(setModified()));

    // random password
    connect(m_rightPanel, SIGNAL(passwordLineEditGotFocus(bool)), m_randomPassword,
        SLOT(setInsertEnabled(bool)));
    connect(m_randomPassword, SIGNAL(insertPassword(const QString&)),
        SIGNAL(insertPassword(const QString&)));

    // auto logout
    connect(dynamic_cast<TimeoutApplication*>(qApp), SIGNAL(timedOut()), SLOT(logout()));

    // previously I used a hidden action for this, but this doesn't work in Qt4 any more
    QShortcut* focusSearch = new QShortcut(QKeySequence(Qt::CTRL|Qt::Key_G), this);
    connect(focusSearch, SIGNAL(activated()), m_searchCombo, SLOT(setFocus()));
}


/**
 * @brief Creates a Icon in Qpamat
 *
 * Helper function to create a QIcon that contains both a small icon with 16x16
 * pixels and a large icon with 24x24 pixels. The @p qpamatName will be appended
 * with <tt>"_16.png"</tt> for the small icon and with <tt>"_24.png"</tt> for
 * the large icon. In both cases, it uses the path <tt>":/images/"</tt> for icon
 * lookup.
 *
 * When @p freedesktopName is set, the function QIcon::fromTheme() will be used
 * with the icon build from @p qpamatName as fallback.  For the freedesktop
 * icon names, see http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html.
 *
 * @param[in] qpamatName the qpamat name of the icon which will be used to build
 *                       the fallback icon with the rules described above.
 * @param[in] freedesktopName the name in the freedesktop spec
 *
 * @return the QIcon object
 */
QIcon QpamatWindow::createIcon(const QString &qpamatName, const QString &freedesktopName)
{
    QString smallIcon = qpamatName + "_16.png";
    QString largeIcon = qpamatName + "_24.png";

    QIcon fallbackIcon(QPixmap(":/images/" + smallIcon));
    fallbackIcon.addPixmap(QPixmap(":/images/" + largeIcon));

    return QIcon::fromTheme(freedesktopName, fallbackIcon);
}

/**
 * @brief Initializes the actions.
 */
void QpamatWindow::initActions()
{
    // ----- File ----------------------------------------------------------------------------------
    m_actions.newAction = new QAction(createIcon("stock_new", "document-new"), tr("&New"), this);
    m_actions.newAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_N));
    m_actions.quitAction = new QAction(createIcon("stock_exit", "application-exit"), tr("E&xit"),this);
    m_actions.quitAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Q));
    m_actions.quitActionNoKeyboardShortcut = new QAction(
        createIcon("stock_exit", "application-exit"), tr("E&xit"), this);
    m_actions.loginLogoutAction = new QAction(createIcon("login"), tr("&Login"), this);
    m_actions.loginLogoutAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_L));
    m_actions.saveAction = new QAction(createIcon("stock_save", "document-save"), tr("&Save"), this);
    m_actions.saveAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_S));
    m_actions.exportAction = new QAction(createIcon("export", "document-save-as"),
                                         tr("&Export..."), this);
    m_actions.printAction = new QAction(createIcon("stock_print", "document-print"),
                                        tr("&Print..."), this);
    m_actions.printAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_P));

    // ----- Options -------------------------------------------------------------------------------
    m_actions.changePasswordAction = new QAction(tr("&Change Password..."), this);
    m_actions.settingsAction = new QAction(createIcon("stock_preferences", "preferences-other"),
                                           tr("&Settings..."), this);
    m_actions.settingsAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_T));

    // ----- Extras --------------------------------------------------------------------------------
    m_actions.randomPasswordAction = new QAction(tr("&Random Password..."), this);
    m_actions.randomPasswordAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_R));

    m_actions.passwordStrengthAction = new QAction(tr("&Show password strength"),this);
    m_actions.passwordStrengthAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_W));
    m_actions.passwordStrengthAction->setToggleAction(true);

    QIcon clearClipboardActionIcon(QPixmap(":/images/clear_clipboard_16.png"));
    clearClipboardActionIcon.addPixmap(QPixmap(":/images/clear_clipboard_24.png"));
    m_actions.clearClipboardAction = new QAction(clearClipboardActionIcon,
        tr("&Clear clipboard"), this);
    m_actions.clearClipboardAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_E));

    // ----- Help ----------------------------------------------------------------------------------
    m_actions.helpAction = new QAction(createIcon("stock_help", "system-help"), tr("&Help"), this);
    m_actions.helpAction->setShortcut(QKeySequence(Qt::Key_F1));
    m_actions.aboutAction = new QAction(createIcon("info"), tr("&About..."), this);
    m_actions.aboutQtAction = new QAction(tr("About &Qt..."), this);

    // ------ Toolbar ------------------------------------------------------------------------------
    m_actions.addItemAction = new QAction(createIcon("stock_add", "list-add"), tr("Add item"), this);
    m_actions.addItemAction->setShortcut(QKeySequence(Qt::Key_Insert));
    m_actions.removeItemAction = new QAction(createIcon("stock_remove", "list-remove"), tr("Remove item"), this);

    // ------ Misc ---------------------------------------------------------------------------------
    m_actions.searchAction = new QAction(createIcon("stock_search", "edit-find"), tr("Search"), this);

    m_actions.showHideAction = new QAction(tr("&Show"), this);
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
