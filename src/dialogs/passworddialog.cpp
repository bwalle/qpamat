/*
 * Id: $Id: passworddialog.cpp,v 1.4 2003/12/28 22:08:15 bwalle Exp $
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
#include <qpushbutton.h>

#include "passworddialog.h"

/*!
    \class PasswordDialog
    
    \brief Dialog to enter the password
    
    This dialog is just used to enter a password. It's the dialog that is shown on startup and
    lets the user type-in the password. No checking is done here.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.4 $
    \date $Date: 2003/12/28 22:08:15 $
*/

/*!
    Creates a new instance of a PasswordDialog.
    \param parent the parent widget
    \param name the name of the widget
*/
PasswordDialog::PasswordDialog(QWidget* parent, const char* name)
    : QDialog(parent, name)
{
    setCaption("QPaMaT");
    
    // create elements
    QLabel* label = new QLabel(tr("Enter the passprase:"), this);
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumWidth(250);
    
    QPushButton* okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    
    QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
    
    // create layouts
    QVBoxLayout* layout = new QVBoxLayout(this, 6, 6);
    QHBoxLayout* buttonLayout = new QHBoxLayout(0, 0, 6);
    
    // layout elements
    buttonLayout->addStretch(10);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    layout->addWidget(label);
    layout->addWidget(m_passwordEdit);
    layout->addStretch(10);
    layout->addSpacing(7);
    layout->addLayout(buttonLayout);
    
    // communication
    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}


/*!
    Returns the password the user has entered.
    \return the password
*/
QString PasswordDialog::getPassword() const
{
    return m_passwordEdit->text();
}

