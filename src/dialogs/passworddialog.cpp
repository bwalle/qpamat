/*
 * Id: $Id: passworddialog.cpp,v 1.6 2004/01/02 12:20:57 bwalle Exp $
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
#include "qpamat.h"

/*!
    \class PasswordDialog
    
    \brief Dialog to enter the password
    
    This dialog is just used to enter a password. It's the dialog that is shown on startup and
    lets the user type-in the password. No checking is done here.
    
    It grabs the keyboard unless the user turned it off with the global setting 
    <tt>Password/NoGrabbing</tt>.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.6 $
    \date $Date: 2004/01/02 12:20:57 $
*/

/*!
    Creates a new instance of a PasswordDialog.
    \param parent the parent widget
    \param name the name of the widget
*/
PasswordDialog::PasswordDialog(QWidget* parent, const char* name)
    : QDialog(parent, name, false, WStyle_Customize | WStyle_NormalBorder |
                                   WStyle_Title | WStyle_SysMenu)
{
    setCaption("QPaMaT");
    
    // create elements
    QLabel* label = new QLabel(tr("Enter the passprase:"), this);
    
    m_passwordEdit = new FocusLineEdit(this);
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
    
    if (!qpamat->set().readBoolEntry("Password/NoGrabbing"))
    {
        connect(m_passwordEdit, SIGNAL(gotFocus()), SLOT(grab()));
        connect(m_passwordEdit, SIGNAL(lostFocus()), SLOT(release()));
    }
}


/*!
    Grabs the password edit. Should be connected to the gotFocus() signal of the password
    lineedit widget.
*/
void PasswordDialog::grab()
{
    m_passwordEdit->grabKeyboard();
}


/*!
    Releases the password edit. Should be connected to the lostFocus() signal of the password
    lineedit widget.
*/
void PasswordDialog::release()
{
    if (QWidget* widget = keyboardGrabber())
    {
        widget->releaseKeyboard();
    }
}


/*!
    Returns the password the user has entered.
    \return the password
*/
QString PasswordDialog::getPassword() const
{
    return m_passwordEdit->text();
}

