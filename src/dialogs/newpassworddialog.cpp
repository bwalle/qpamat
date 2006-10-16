/*
 * Id: $Id$
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
#include <QDialog>
#include <QLineEdit>
#include <QLayout>
#include <Q3HBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "newpassworddialogprivate.h"
#include "newpassworddialog.h"
#include "qpamat.h"
#include "security/masterpasswordchecker.h"
#include "settings.h"

/*!
    \class NewPasswordDialog
    
    \brief Password to enter a new password for new files.
    
    This dialog was made to enter a new password or to change the existing password. The
    mode depends on the argument \p oldPassword of the constructor. If it is QString::null,
    a new password can be made.
    
    This class uses not the dictionary-based checker but the MasterPasswordChecker. This is
    because maybe the user has not setup a dictionary at this time and because here the
    user cannot use a random password but must memorize the password.
    
    \ingroup dialogs
    \author Bernhard Walle
    \version $Revision: 1.12 $
    \date $Date$
*/


/*!
    Creates a new instance of a NewPasswordDialog.
    \param parent the parent widget
    \param oldPassword the old password (the user has to enter it and it must be right) or
                       QString::null if a new password should be entered.
*/
NewPasswordDialog::NewPasswordDialog(QWidget* parent, const QString& oldPassword)
    : QDialog(parent), m_oldPassword(oldPassword)
{
    setCaption("QPaMaT");
    
    createAndLayout();
    
    // validators
    PasswordValidator* validator = new PasswordValidator(this);
    m_firstPasswordEdit->setValidator(validator);
    m_secondPasswordEdit->setValidator(validator);
    
    // communication
    connect(m_okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(m_firstPasswordEdit, SIGNAL(textChanged(const QString&)), SLOT(checkOkEnabled()));
    connect(m_secondPasswordEdit, SIGNAL(textChanged(const QString&)), SLOT(checkOkEnabled()));
    
    if (!qpamat->set().readBoolEntry("Password/NoGrabbing"))
    {
        if (!m_oldPassword.isNull())
        {
            connect(m_oldPasswordEdit, SIGNAL(gotFocus()), SLOT(grabOldPassword()));
            connect(m_oldPasswordEdit, SIGNAL(lostFocus()), SLOT(release()));
        }
        
        connect(m_firstPasswordEdit, SIGNAL(gotFocus()), SLOT(grabFirstPassword()));
        connect(m_firstPasswordEdit, SIGNAL(lostFocus()), SLOT(release()));
        connect(m_secondPasswordEdit, SIGNAL(gotFocus()), SLOT(grabSecondPassword()));
        connect(m_secondPasswordEdit, SIGNAL(lostFocus()), SLOT(release()));
    }
}

/*!
    Creates and layout the widgets of the dialog.
*/
void NewPasswordDialog::createAndLayout()
{
    // text fields
    if (!m_oldPassword.isNull())
    {
        m_oldPasswordEdit = new FocusLineEdit(this);
        m_oldPasswordEdit->setEchoMode(QLineEdit::Password);
        m_oldPasswordEdit->setMinimumWidth(250);
    }
    
    m_firstPasswordEdit = new FocusLineEdit(this);
    m_firstPasswordEdit->setEchoMode(QLineEdit::Password);
    m_firstPasswordEdit->setMinimumWidth(250);
    
    m_secondPasswordEdit = new FocusLineEdit(this);
    m_secondPasswordEdit->setEchoMode(QLineEdit::Password);
    m_secondPasswordEdit->setMinimumWidth(250);
    
    // TODO: autowrapping doesn't work, spaces are too high, need to investigate ...
    const QString changeText = tr("Enter the <b>old passphrase</b> once and the <b>new "
        "passphrase</b><br> two times for verification. The minimum length must be six<br> characters, "
        "but for security reasons more than two words<br> are good!");
    const QString newText = tr("Enter the <b>passphrase</b> two times for verification. The "
        "minimum length<br> must be six characters, but for security reasons more than two<br> "
        "words are good!");
    
    // labels
    QLabel* label = new QLabel((m_oldPassword.isNull() ? newText : changeText), this);
    QLabel* old = 0;
    if (!m_oldPassword.isNull())
    {
        old = new QLabel(tr("&Old password:"), this);
        old->setBuddy(m_oldPasswordEdit);
    }
    QLabel* first = new QLabel(tr("&Password:"), this);
    first->setBuddy(m_firstPasswordEdit);
    QLabel* second = new QLabel(tr("&Verification:"), this);
    second->setBuddy(m_secondPasswordEdit);
    
    
    // buttons
    QDialogButtonBox *dialogButtons = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, this);
    m_okButton = dialogButtons->button(QDialogButtonBox::Ok);
    m_okButton->setEnabled(false);
    m_okButton->setDefault(true);
    m_cancelButton = dialogButtons->button(QDialogButtonBox::Cancel);
    
    
    // create layouts
    QVBoxLayout* layout = new QVBoxLayout(this);
    QGridLayout* textfields = new QGridLayout(0);//, 2, (m_oldPassword.isNull() ? 2 : 3), 0, 6);
    
    
    int i = 0;
    if (!m_oldPassword.isNull())
    {
        textfields->addWidget(old, 0, 0);
        textfields->addWidget(m_oldPasswordEdit, 0, 1);
        ++i;
    }
    textfields->addWidget(first, i+0, 0);
    textfields->addWidget(m_firstPasswordEdit, i+0, 1);
    textfields->addWidget(second, i+1, 0);
    textfields->addWidget(m_secondPasswordEdit, i+1, 1);
    
    layout->addWidget(label);
    layout->addLayout(textfields);
    layout->addSpacing(7);
    layout->addWidget(dialogButtons);
}


/*!
    Method that is called if the user presses Ok.
    It checks the password for quality and if the old password was right. If not, it displays
    a message box and the user has the chance to try again.
*/
void NewPasswordDialog::accept()
{
    if (m_secondPasswordEdit->text()!= m_secondPasswordEdit->text())
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("The confirmation passphrase was not the same as the first one. "
               "Re-enter the confirmation or both.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        return;
    }
    if (!m_oldPassword.isNull() && m_oldPassword != m_oldPasswordEdit->text())
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("The old password was incorrect. Without the old password, "
               "the password cannot be changed.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        m_oldPasswordEdit->setFocus();
        m_oldPasswordEdit->selectAll();
        return;
    }
    
    MasterPasswordChecker checker;
    const QString& password = m_secondPasswordEdit->text();
    bool ok;
    
    try
    {
        double quality = checker.passwordQuality(password);
        ok = quality > qpamat->set().readDoubleEntry("Security/StrongPasswordLimit");
    }
    catch (const std::exception& exc)
    {
        QMessageBox::warning(this, "QPaMaT",
            ("<qt>"+tr("An error occurred while checking the password:<br>%1")+"</qt>").
            arg(exc.what()),
            QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        return;
    }
    
    if (!ok)
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("<nobr>The passphrase is too simple. See the</nobr> Security settings "
               "for details about the conditions a valid password must met.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        m_firstPasswordEdit->setFocus();
        return;
    }
    
    QDialog::accept();
}

/*!
    Grabs the old password edit. Should be connected to the gotFocus() signal of the password
    lineedit widget.
*/
void NewPasswordDialog::grabOldPassword()
{
    m_oldPasswordEdit->grabKeyboard();
}


/*!
    Grabs the first password edit. Should be connected to the gotFocus() signal of the password
    lineedit widget.
*/
void NewPasswordDialog::grabFirstPassword()
{
    m_firstPasswordEdit->grabKeyboard();
}


/*!
    Grabs the second password edit. Should be connected to the gotFocus() signal of the password
    lineedit widget.
*/
void NewPasswordDialog::grabSecondPassword()
{
    m_secondPasswordEdit->grabKeyboard();
}


/*!
    Releases the password edit. Should be connected to the lostFocus() signal of the password
    lineedit widget
*/
void NewPasswordDialog::release()
{
    QWidget* widget = keyboardGrabber();
    if (widget)
    {
        widget->releaseKeyboard();
    }
}


/*!
    Returns the (new) password the user has entered.
    \return the password
*/
QString NewPasswordDialog::getPassword() const
{
    return m_firstPasswordEdit->text();
}


/*!
    Checks the user's input and enables the Ok button if necessary. This slot is called always
    if the users changes the input.
*/
void NewPasswordDialog::checkOkEnabled() const
{
    if (m_firstPasswordEdit->hasAcceptableInput() && m_secondPasswordEdit->hasAcceptableInput()
            && m_firstPasswordEdit->text().length() == m_secondPasswordEdit->text().length())
    {
        m_okButton->setEnabled(true);
    }
    else
    {
        m_okButton->setEnabled(false);
    } 
}

#ifndef DOXYGEN

// -------------------------------------------------------------------------------------------------
//                                     Validator
// -------------------------------------------------------------------------------------------------

/*!
    \class PasswordValidator
    
    \brief Validator for the passwords in the NewPasswordDialog dialog.
    
    The validator simply checks if the password has less than six characters. It is used to
    enable or disable the Ok button.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.12 $
    \date $Date$
*/

/*!
    Creates a new instance of a PasswordValidator.
    \param parent the parent widget
    \param name the name of the widget
*/
PasswordValidator::PasswordValidator(QObject* parent, const char* name)
    : QValidator(parent, name)
{}


/*!
    Does the validation. It returns QValidator::Intermediate if the password has less than
    six characters and QValidator::Acceptable if it has equal or more than six.
    \param input the input
    \param pos the position of the inserted stuff (unused here)
*/
PasswordValidator::State PasswordValidator::validate(QString& input, int& pos) const
{
    UNUSED(pos);

    if (input.length() < 6)
    {
        return QValidator::Intermediate;
    }
    else
    {
        return QValidator::Acceptable;
    }
}

#endif // DOXYGEN
