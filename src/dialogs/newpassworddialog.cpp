/*
 * Id: $Id: newpassworddialog.cpp,v 1.2 2003/10/20 20:53:46 bwalle Exp $
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
#include <qdialog.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

#include "newpassworddialog.h"

// -------------------------------------------------------------------------------------------------
NewPasswordDialog::NewPasswordDialog(QWidget* parent, const QString& oldPassword)
// -------------------------------------------------------------------------------------------------
    : QDialog(parent), m_oldPassword(oldPassword)
{
    setCaption("QPaMaT");
    
    createAndLayout();
    
    // validators
    PasswordValidator* validator = new PasswordValidator(this);
    m_firstPasswordEdit->setValidator(validator);
    m_secondPasswordEdit->setValidator(validator);
    
    
    // communication
    QObject::connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(m_firstPasswordEdit, SIGNAL(textChanged(const QString&)), 
        this, SLOT(checkOkEnabled()));
    QObject::connect(m_secondPasswordEdit, SIGNAL(textChanged(const QString&)), 
        this, SLOT(checkOkEnabled()));
}

// -------------------------------------------------------------------------------------------------
void NewPasswordDialog::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // text fields
    if (m_oldPassword)
    {
        m_oldPasswordEdit = new QLineEdit(this);
        m_oldPasswordEdit->setEchoMode(QLineEdit::Password);
        m_oldPasswordEdit->setMinimumWidth(250);
    }
    
    m_firstPasswordEdit = new QLineEdit(this);
    m_firstPasswordEdit->setEchoMode(QLineEdit::Password);
    m_firstPasswordEdit->setMinimumWidth(250);
    
    m_secondPasswordEdit = new QLineEdit(this);
    m_secondPasswordEdit->setEchoMode(QLineEdit::Password);
    m_secondPasswordEdit->setMinimumWidth(250);
    
    const QString changeText = "<qt>"+tr("Enter the <b>old passphrase</b> once and the <b>new "
        "passphrase</b> two times for verification. The minimum length must be six characters, "
        "but for security reasons more than two words are good!")+"</qt>";
    const QString newText = "<qt>"+tr("Enter the <b>passphrase</b> two times for verification. The "
        "minimum length must be six characters, but for security reasons more than two "
        "words are good!")+"</qt>";
    
    // labels
    QLabel* label = new QLabel((m_oldPassword ? changeText : newText), this);
    QLabel* old = 0;
    if (m_oldPassword)
    {
        old = new QLabel(tr("&Old password:"), this);
        old->setBuddy(m_oldPasswordEdit);
    }
    QLabel* first = new QLabel(tr("&Password:"), this);
    first->setBuddy(m_firstPasswordEdit);
    QLabel* second = new QLabel(tr("&Verification:"), this);
    second->setBuddy(m_secondPasswordEdit);
    
    
    // buttons
    m_okButton = new QPushButton(tr("OK"), this);
    m_okButton->setDefault(true);
    m_okButton->setEnabled(false);
    m_cancelButton = new QPushButton(tr("Cancel"), this);
    
    
    // create layouts
    QVBoxLayout* layout = new QVBoxLayout(this, 6, 6);
    QGridLayout* textfields = new QGridLayout(0, 2, (m_oldPassword ? 3 : 2), 0, 6);
    QHBoxLayout* buttonLayout = new QHBoxLayout(0, 0, 6);
    
    
    // layout elements
    buttonLayout->addStretch(10);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    
    int i = 0;
    if (m_oldPassword)
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
    layout->addStretch(10);
    layout->addSpacing(7);
    layout->addLayout(buttonLayout);
}


// -------------------------------------------------------------------------------------------------
void NewPasswordDialog::accept()
// -------------------------------------------------------------------------------------------------
{
    if (m_secondPasswordEdit->text()!= m_secondPasswordEdit->text())
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("The confirmation passphrase was not the same as the first one. "
               "Re-enter the confirmation or both.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        return;
    }
    if (m_oldPassword && m_oldPassword != m_oldPasswordEdit->text())
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("The old password was incorrect. Without the old password "
               "the password cannot be changed.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        m_oldPasswordEdit->setFocus();
        m_oldPasswordEdit->selectAll();
        return;
    }
    
    bool uppercase = false;
    bool lowercase = false;
    bool nonLetter = false;
    const QString& password = m_secondPasswordEdit->text();
    for (uint i = 0; i < password.length(); ++i)
    {
        const QChar character = password[i];
        QChar::Category cat = character.category();
        if (cat == QChar::Letter_Uppercase)
        {
            uppercase = true;
        } 
        else if (cat == QChar::Letter_Lowercase)
        {
            lowercase = true;
        }
        else if (! character.isLetter())
        {
            nonLetter = true;
        }
    }
    
    if (!( uppercase && lowercase && nonLetter))
    {
        QMessageBox::warning(this, "QPaMaT",
               "<qt>"+tr("The passphrase is too simple. It must contain at least one uppercase "
               "character, one lowercase character and one non-letter character.")+"</qt>",
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        m_firstPasswordEdit->setFocus();
        return;
    }
    QDialog::accept();
}

// -------------------------------------------------------------------------------------------------
QString NewPasswordDialog::getPassword() const
// -------------------------------------------------------------------------------------------------
{
    return m_firstPasswordEdit->text();
}


// -------------------------------------------------------------------------------------------------
void NewPasswordDialog::checkOkEnabled() const
// -------------------------------------------------------------------------------------------------
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

// =================================================================================================

// -------------------------------------------------------------------------------------------------
PasswordValidator::PasswordValidator(QObject* parent, const char* name)
// -------------------------------------------------------------------------------------------------
        : QValidator(parent, name)
{}


// -------------------------------------------------------------------------------------------------
PasswordValidator::State PasswordValidator::validate(QString& input, int&) const
// -------------------------------------------------------------------------------------------------
{
    if (input.length() < 6)
    {
        return QValidator::Intermediate;
    }
    else
    {
        return QValidator::Acceptable;
    }
}

