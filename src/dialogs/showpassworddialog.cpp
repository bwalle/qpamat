/*
 * Id: $Id: showpassworddialog.cpp,v 1.1 2003/12/21 20:29:07 bwalle Exp $
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
#include <qframe.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qpushbutton.h>

#include "../qpamat.h"
#include "showpassworddialog.h"

// -------------------------------------------------------------------------------------------------
ShowPasswordDialog::ShowPasswordDialog(QWidget* parent, DialogType type, const char* name)
// -------------------------------------------------------------------------------------------------
    : QDialog(parent, name)
{
    setCaption("QPaMaT");
    
    // create elements
    QString labelText = type == TNormalPasswordDlg 
        ? tr("The password is:") 
        : tr("The random password is:");
    
    QLabel* label = new QLabel(labelText, this);
    
    m_passwordEdit = new CopyLabel(qpamat->set().readBoolEntry("Presentation/HideRandomPass"), 
        this);
    m_passwordEdit->setMinimumWidth(250);
    m_passwordEdit->setFocusPolicy(NoFocus);
    
    QPushButton* closeButton = new QPushButton(tr("&Close"), this);
    closeButton->setDefault(true);
    
    QPushButton* insertButton = 0;
    
    if (type == TRandomPasswordDlgInsert)
    {
        insertButton = new QPushButton(tr("&Insert"), this);
    }
    
    // create layouts
    QVBoxLayout* layout = new QVBoxLayout(this, 6, 6);
    QHBoxLayout* buttonLayout = new QHBoxLayout(0, 0, 6);
    
    // layout elements
    buttonLayout->addStretch(10);
    if (insertButton)
    {
        buttonLayout->addWidget(insertButton);
    }
    buttonLayout->addWidget(closeButton);
    
    layout->addWidget(label);
    layout->addWidget(m_passwordEdit);
    layout->addStretch(10);
    layout->addSpacing(7);
    layout->addLayout(buttonLayout);
    
    
    // communication
    connect(closeButton, SIGNAL(clicked()), SLOT(reject()));
    if (insertButton)
    {
        connect(insertButton, SIGNAL(clicked()), SLOT(insertButtonHandler()));
    }
}


// -------------------------------------------------------------------------------------------------
QString ShowPasswordDialog::getPassword() const
// -------------------------------------------------------------------------------------------------
{
    return m_passwordEdit->getContent();
}


// -------------------------------------------------------------------------------------------------
void ShowPasswordDialog::setPassword(const QString& newPassword)
// -------------------------------------------------------------------------------------------------
{
    m_passwordEdit->setContent(newPassword);
}


// -------------------------------------------------------------------------------------------------
void ShowPasswordDialog::insertButtonHandler()
// -------------------------------------------------------------------------------------------------
{
    emit insertPassword(m_passwordEdit->getContent());
}


// -------------------------------------------------------------------------------------------------
void ShowPasswordDialog::copyPassword()
// -------------------------------------------------------------------------------------------------
{
    QClipboard* cb = QApplication::clipboard();
    cb->setText(m_passwordEdit->getContent(), QClipboard::Clipboard);
    cb->setText(m_passwordEdit->getContent(), QClipboard::Selection);
}

