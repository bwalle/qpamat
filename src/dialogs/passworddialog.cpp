/*
 * Id: $Id: passworddialog.cpp,v 1.2 2003/10/12 15:10:33 bwalle Exp $
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

#include "../globals.h"
#include "passworddialog.h"

// -------------------------------------------------------------------------------------------------
PasswordDialog::PasswordDialog(QWidget* parent)
// -------------------------------------------------------------------------------------------------
    : QDialog(parent)
{
    setCaption(QPAMAT_USERVISIBLE);
    
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
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


// -------------------------------------------------------------------------------------------------
QString PasswordDialog::getPassword() const
// -------------------------------------------------------------------------------------------------
{
    return m_passwordEdit->text();
}

