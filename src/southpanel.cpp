/*
 * Id: $Id: southpanel.cpp,v 1.7 2003/12/17 23:17:42 bwalle Exp $
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
#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>

#include "qpamat.h"
#include "southpanel.h"
#include "settings.h"
#include "../images/down_16x16.xpm"
#include "../images/down_22x22.xpm"
#include "../images/up_16x16.xpm"
#include "../images/up_22x22.xpm"


// -------------------------------------------------------------------------------------------------
SouthPanel::SouthPanel(QWidget* parent) 
// -------------------------------------------------------------------------------------------------
    : QFrame(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this, 10, 10, "SouthPanel-QHBoxLayout");
    QGroupBox* group = new QGroupBox(2, Horizontal, QString::null, this, "SouthPanel-GroupBox");
    
    // Type
    QLabel* typeLabel = new QLabel(tr("&Type"), group);
    m_typeCombo = new QComboBox(false, group);
    m_typeCombo->setMaximumWidth(150);
    m_typeCombo->insertItem(tr("Misc"));            // the order must match the enumeration in
    m_typeCombo->insertItem(tr("Username"));        // property.h!!
    m_typeCombo->insertItem(tr("Password"));
    m_typeCombo->insertItem(tr("URL"));
    typeLabel->setBuddy(m_typeCombo);
    
    // Key
    QLabel* keyLabel = new QLabel(tr("&Key"), group);
    m_keyLineEdit = new QLineEdit(group);
    keyLabel->setBuddy(m_keyLineEdit);
    
    // Value
    QLabel* valueLabel = new QLabel(tr("&Value"), group);
    m_valueLineEdit = new FocusLineEdit(group);
    valueLabel->setBuddy(m_valueLineEdit);
    
    hLayout->addWidget(group);
    
    // button group box
    QWidget* vbox = new QWidget(this, "SouthPanel vertical box");
    QVBoxLayout* vboxLayout = new QVBoxLayout(vbox, 0, 6, "Layout of vbox");
    
    // up button
    m_upButton = new QToolButton(vbox, "SouthPanel up button");
    m_upButton->setIconSet(QIconSet(up_16x16_xpm, up_22x22_xpm));
    m_upButton->setUsesBigPixmap(true);
    
    // down button
    m_downButton = new QToolButton(vbox, "SouthPanel down button");
    m_downButton->setIconSet(QIconSet(down_16x16_xpm, down_22x22_xpm));
    m_downButton->setUsesBigPixmap(true);
    
    // layout for the button group box
    vboxLayout->addWidget(m_upButton);
    vboxLayout->addWidget(m_downButton);
    vboxLayout->addStretch(5);
        
    hLayout->addWidget(vbox);
    
    setEnabled(false);
    setFocusPolicy(QWidget::NoFocus);
    
    // disable the two buttons
    m_upButton->setEnabled(false);
    m_downButton->setEnabled(false);
    
    connect(m_typeCombo, SIGNAL(activated(int)), SLOT(updateData()));
    connect(m_typeCombo, SIGNAL(activated(int)), SLOT(comboBoxChanged(int)));
    connect(m_keyLineEdit, SIGNAL(textChanged(const QString&)), SLOT(updateData()));
    connect(m_valueLineEdit, SIGNAL(textChanged(const QString&)), SLOT(updateData()));
    connect(m_upButton, SIGNAL(pressed()), SIGNAL(moveUp()));
    connect(m_downButton, SIGNAL(pressed()), SIGNAL(moveDown()));
    connect(m_valueLineEdit, SIGNAL(gotFocus()), SLOT(focusInValueHandler()));
    connect(m_valueLineEdit, SIGNAL(lostFocus()), SLOT(focusOutValueHandler()));
    connect(m_typeCombo, SIGNAL(activated(int)), SIGNAL(stateModified()));
    connect(m_keyLineEdit, SIGNAL(textChanged(const QString&)), SIGNAL(stateModified()));
    connect(m_valueLineEdit, SIGNAL(textChanged(const QString&)), SIGNAL(stateModified()));
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::clear()
// -------------------------------------------------------------------------------------------------
{
    m_currentProperty = 0;
     
    m_keyLineEdit->setText(QString::null);
    m_valueLineEdit->setText(QString::null);
    m_typeCombo->setCurrentItem(0);
    m_valueLineEdit->setEchoMode(QLineEdit::Normal);
    m_oldComboValue = -1;
    setEnabled(false);
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::setItem (Property* property)
// -------------------------------------------------------------------------------------------------
{
    clear();
    
    if (property != 0)
    {
        m_typeCombo->setCurrentItem(property->getType());
        m_valueLineEdit->setText(property->getValue());
        m_keyLineEdit->setText(property->getKey());
        m_oldComboValue = property->getType();
        if (property->isHidden())
        {
            m_valueLineEdit->setEchoMode(QLineEdit::Password);
        }
        insertAutoText();
    }
    m_currentProperty = property;
    
    setEnabled(property != 0);
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::updateData()
// -------------------------------------------------------------------------------------------------
{
    if (m_currentProperty != 0)
    {
        m_currentProperty->setType(Property::Type(m_typeCombo->currentItem()));
        m_currentProperty->setHidden(m_typeCombo->currentItem() == Property::PASSWORD);
        m_currentProperty->setEncrypted(m_typeCombo->currentItem() == Property::PASSWORD);
        m_currentProperty->setValue(m_valueLineEdit->text());
        m_currentProperty->setKey(m_keyLineEdit->text());
        m_valueLineEdit->setEchoMode(
            m_currentProperty->isHidden() 
                ? QLineEdit::Password
                : QLineEdit::Normal );
    }
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::comboBoxChanged(int newChoice)
// -------------------------------------------------------------------------------------------------
{
    
    if (m_oldComboValue != newChoice)
    {
        insertAutoText();
        
        if (m_oldComboValue == Property::PASSWORD)
        {
            m_currentProperty->setValue("");
            m_valueLineEdit->setText("");
        }
    }
    m_oldComboValue = newChoice;
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::insertAutoText()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    
    if (m_keyLineEdit->text().isEmpty())
    {
        QString newTxt;
        switch ( Property::Type(m_typeCombo->currentItem()) )
        {
            case Property::MISC:
                newTxt = set.readEntry("AutoText/Misc", Settings::DEFAULT_AUTOTEXT_MISC);
                break;
            case Property::USERNAME:
                newTxt = set.readEntry("AutoText/Username",Settings::DEFAULT_AUTOTEXT_USERNAME);
                break;
            case Property::PASSWORD:
                newTxt = set.readEntry("AutoText/Password",Settings::DEFAULT_AUTOTEXT_PASSWORD);
                break;
            case Property::URL:
                newTxt = set.readEntry("AutoText/URL", Settings::DEFAULT_AUTOTEXT_URL);
                break;
            default:
                qDebug("SouthPanel::comboBoxChanged: newChoice is out of range\n");
                break;
        }
        m_keyLineEdit->setText(newTxt);
        m_valueLineEdit->setFocus();
    }
}

// -------------------------------------------------------------------------------------------------
bool SouthPanel::isFocusInside() const
// -------------------------------------------------------------------------------------------------
{
    return m_keyLineEdit->hasFocus() || m_typeCombo->hasFocus() || m_valueLineEdit->hasFocus();
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::setMovingEnabled(bool up, bool down)
// -------------------------------------------------------------------------------------------------
{
    m_upButton->setEnabled(up);
    m_downButton->setEnabled(down);
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::focusInValueHandler()
// -------------------------------------------------------------------------------------------------
{
    if (m_currentProperty && m_currentProperty->getType() == Property::PASSWORD)
    {
        passwordLineEditGotFocus(true);
    }
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::focusOutValueHandler()
// -------------------------------------------------------------------------------------------------
{
    if (m_currentProperty && m_currentProperty->getType() == Property::PASSWORD)
    {
        emit passwordLineEditGotFocus(false);
    }
}


// -------------------------------------------------------------------------------------------------
void SouthPanel::insertPassword(const QString& password)
// -------------------------------------------------------------------------------------------------
{
    m_valueLineEdit->insert(password);
}
