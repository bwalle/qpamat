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
#include <QLayout>
#include <QDialog>
#include <QLabel>
#include <Q3HBox>
#include <QPushButton>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "insertcarddialog.h"


/*!
    \class InsertCardDialog
    
    \brief Dialog that is displayed if the user should insert the card
    
    This dialog is displayed if the user should insert the smartcard into the terminal.
    There a two formes of the dialog:
    
     - If the card is PIN-protected, the user should enter a pin. Therefore you
       have to set \p pin to \c true in the constructor. The user can enter a PIN
       containing six hexadecimal characters. 
     - If the card is not PIN-protected, it contains just a message.
    
    In envery case, a Ok button and a Cancel button is displayed.
    
    \ingroup dialogs
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date$
*/


/*!
    Creates a new instance of a InsertCardDialog.
    \param pin \c true if there should be a text field for entering the pin, \c false otherwise
    \param parent the parent widget
    \param name the widget's name
*/
InsertCardDialog::InsertCardDialog(bool pin, QWidget* parent, const char* name)
    : QDialog(parent, name), m_pinEdit(0)
{
    const char* strings[] = {
        QT_TR_NOOP("Insert the card in the reader."),
        QT_TR_NOOP("Enter the PIN and insert the card in your reader.\nIt must consist of "
            "six hexadecimal digits:")
    };
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this, 10,15, "VLayout");   
    QVBoxLayout* rightLayout = new QVBoxLayout(0, 0, 10, "HLayout");
    
    QLabel* leftIcon = new QLabel(this, "left icon");
    leftIcon->setPixmap(QPixmap(":/images/smartcard_48.png"));
   
    QLabel* label = new QLabel(tr(strings[pin]), this, "Label");
    
    Q3HBox* box = new Q3HBox(this, "Box");
    box->setSpacing(8);
    
    if (pin)
    {
        m_pinEdit = new QLineEdit(this, "LineEdit");
        m_pinEdit->setValidator(new CardPINValidator(this, "PINValidator"));
        m_pinEdit->setEchoMode(QLineEdit::Password);
    }
    
    QWidget* dummyLeft = new QWidget(box, "Left dummy");
    m_okButton = new QPushButton(tr("OK"), box, "m_okButton");
    m_okButton->setDefault(true);
    m_cancelButton = new QPushButton(tr("Cancel"), box, "CancelButton");
    
    // no spacing
    m_cancelButton->setAutoDefault(false);
    
    QWidget* dummyRight = 0;
    if (!pin)
    {
        dummyRight = new QWidget(box, "Rigth dummy");
    }
    
    box->setStretchFactor(dummyLeft, 5);
    box->setStretchFactor(m_okButton, 0);
    box->setStretchFactor(m_cancelButton, 0);
    if (!pin)
    {
        box->setStretchFactor(dummyRight, 5);
    }
    
    // layout
    mainLayout->addWidget(leftIcon, 0, Qt::AlignLeft| Qt::AlignTop);
    mainLayout->addLayout(rightLayout, 5);
    
    rightLayout->addSpacing(3);
    rightLayout->addWidget(label, 5, Qt::AlignHCenter | Qt::AlignTop);
    if (pin)
    {
        rightLayout->addWidget(m_pinEdit);
    }
    rightLayout->addSpacing(10);
    rightLayout->addStretch();
    rightLayout->addWidget(box);
    
    connect(m_okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), SLOT(reject()));
    
    if (pin)
    {
        connect(m_pinEdit, SIGNAL(textChanged(const QString&)), 
            SLOT(pinEditHandler(const QString&)));
        m_okButton->setEnabled(false);
    }
}

/*!
    Returns the entered PIN. 
    \return the pin or QString::null if there's no PIN. The result is only validated if the
            status of the dialog is Ok.
*/
QString InsertCardDialog::getPIN() const
{
    return m_pinEdit ? m_pinEdit->text() : QString::null;
}


/*!
    Handler if the user changed the value of the PIN line edit. Checks if the value is
    valid and enables the Ok button if needed.
    \param text the current text
*/
void InsertCardDialog::pinEditHandler(const QString& text)
{
    Q_ASSERT( m_pinEdit );
    
    m_okButton->setEnabled(m_pinEdit->hasAcceptableInput());
}
