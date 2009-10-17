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
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QCursor>

#include "qpamatwindow.h"
#include "qpamat.h"
#include "global.h"
#include "settings.h"
#include "dialogs/showpassworddialog.h"
#include "randompassword.h"
#include "security/passwordgeneratorfactory.h"
#include "security/hybridpasswordchecker.h"


/**
 * @class RandomPassword
 *
 * @brief This class handles RandomPasswords in the QPaMaT application.
 *
 * It doesn't generate RandomPasswords but it manages displaying and sending
 * them to other widgets.
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @fn RandomPassword::insertPassword(const QString&)
 *
 * This signal is emitted every time the user wants to insert the password at
 * current position.
 *
 * @param password the password
 */

/**
 * @brief Creates a new instance of a RandomPassword object.
 *
 * Inserting is not enabled initally.
 *
 * @param parent the parent object
 * @param name the name of the object
 */
RandomPassword::RandomPassword(QpamatWindow* parent, const char* name)
    : QObject(parent, name), m_insertEnabled(false), m_parent(parent)
{ }


/**
 * @brief Enables the insert function.
 *
 * @param enabled if the insert function should be enabled
 */
void RandomPassword::setInsertEnabled(bool enabled)
{
    m_insertEnabled = enabled;
}


/**
 * @brief Call this slot if you want a random password.
 *
 * A password dialog is displayed that lets the user copy the password into
 * the clipboard or insert (if possible) at the current password QLineEdit.
 */
void RandomPassword::requestPassword()
{
    PasswordGenerator* passwordgen = 0;
    QpamatWindow *win = Qpamat::instance()->getWindow();
    QString allowed = win->set().readEntry("Security/AllowedCharacters");
    PasswordChecker* checker = 0;
    try {
        checker = new HybridPasswordChecker(win->set().readEntry("Security/DictionaryFile"));
        passwordgen = PasswordGeneratorFactory::getGenerator(
            win->set().readEntry( "Security/PasswordGenerator" ),
            win->set().readEntry( "Security/PasswordGenAdditional" )
        );
    } catch (const std::exception& exc) {
        QMessageBox::warning(m_parent, "QPaMaT",
                tr("Failed to create a password checker or generator:\n\n%1\n\nAdjust the settings!")
                .arg(exc.what()), QMessageBox::Ok, QMessageBox::NoButton);
        delete passwordgen;
        delete checker;
        return;
    }

    if (passwordgen->isSlow())
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    bool ok = false;
    QString password;

    for (int i = 0; !ok && i < 200; ++i) {

        try {
            password = passwordgen->getPassword(
                win->set().readNumEntry("Security/Length"),
                win->set().readEntry("Security/AllowedCharacters")
            );
            double quality = checker->passwordQuality(password);
            ok = quality > win->set().readDoubleEntry("StrongPasswordLimit");

        } catch (const std::exception& exc) {
            if (passwordgen->isSlow())
                QApplication::restoreOverrideCursor();

            QMessageBox::warning(m_parent, "QPaMaT",
                tr("<qt><nobr>An error occurred while generating the password:</nobr><br>%1</qt>")
                .arg(exc.what()), QMessageBox::Ok, QMessageBox::NoButton);
            break;
        }
    }

    if (passwordgen->isSlow())
        QApplication::restoreOverrideCursor();

    ShowPasswordDialog::DialogType dialogType = m_insertEnabled
            ? ShowPasswordDialog::TRandomPasswordDlgInsert
            : ShowPasswordDialog::TRandomPasswordDlg;

    ShowPasswordDialog* dlg = new ShowPasswordDialog(m_parent, dialogType, "RandomPwDialog");
    if (ok) {
        dlg->setPassword(password);
        connect(dlg, SIGNAL(insertPassword(const QString&)), SIGNAL(insertPassword(const QString&)));
        dlg->exec();
    } else
        QMessageBox::information(m_parent, "QPaMaT", tr("<qt><nobr>Failed to create a random "
            "password with the current</nobr> generator and the current password checker. "
            "Adjust the settings!"), QMessageBox::Ok, QMessageBox::NoButton);

    delete dlg;
    delete passwordgen;
    delete checker;
}

// vim: set sw=4 ts=4 et:
