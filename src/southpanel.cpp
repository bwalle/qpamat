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
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <Q3VBox>
#include <Q3ButtonGroup>
#include <QToolTip>
#include <QToolButton>
#include <QMessageBox>
#include <Q3Frame>
#include <QPixmap>
#include <QHBoxLayout>

#include "qpamat.h"

#include "southpanel.h"
#include "settings.h"
#include "util/stringdisplay.h"


/*!
    \class SouthPanel

    \brief Represents the south panel.

    \ingroup gui
    \author Bernhard Walle
*/

/*!
    \fn SouthPanel::moveUp

    This signal is emitted if the user pressed the "Up" button to move an item one
    step up.
*/

/*!
    \fn SouthPanel::moveDown

    This signal is emitted if the user pressed the "Down" button to move an item one
    step down.
*/

/*!
    \fn SouthPanel::passwordLineEditGotFocus(bool)

    This signal is emitted if the password field got this focus.
    \param focus \c true if it got the focus, \c false if the focus is lost
*/

/*!
    \fn SouthPanel::stateModified()

    If something was modified, need to determine if saving is necessary.
*/

/*!
    \fn SouthPanel::passwordStrengthUpdated()

    This signal is emitted if the password strength of an item has changed an therefore
    the displaying must be updated.
*/

/*!
    Creates a new instance of the south panel.
    \param parent the parent
*/
SouthPanel::SouthPanel(QWidget* parent)
    : Q3Frame(parent), m_lastStrength(Property::PUndefined)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this, 0, 10, "SouthPanel-QHBoxLayout");
    Q3GroupBox* group = new Q3GroupBox(2, Qt::Horizontal, tr("&Properties"), this, "SouthPanel-GroupBox");

    m_updatePasswordQualityTimer = new QTimer(this, "Timer");

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
    Q3VBox* vbox = new Q3VBox(this, "SouthPanel vertical box");
    vbox->setSpacing(6);

    // up button
    m_upButton = new QToolButton(vbox, "SouthPanel up button");
    QIcon upIcon(QPixmap(":/images/stock_up_arrow_16.png"));
    upIcon.addPixmap(QPixmap(":/images/stock_up_arrow_24.png"));
    m_upButton->setIconSet(upIcon);
    //m_upButton->setUsesBigPixmap(true);
    m_upButton->setIconSize(QSize(22, 22));

    // down button
    m_downButton = new QToolButton(vbox, "SouthPanel down button");
    QIcon downIcon(QPixmap(":/images/stock_down_arrow_16.png"));
    downIcon.addPixmap(QPixmap(":/images/stock_down_arrow_24.png"));
    m_downButton->setIconSet(downIcon);
    m_downButton->setIconSize(QSize(22, 22));
    //m_downButton->setUsesBigPixmap(true);

    // filler widget
    QWidget* filler = new QWidget(vbox);
    vbox->setStretchFactor(filler, 1);

    // indicator
    m_indicatorLabel = new QLabel(vbox, "Indicator label", Qt::WRepaintNoErase);
    m_indicatorLabel->setAlignment(Qt::AlignHCenter);
    m_indicatorLabel->setFixedHeight(28);

    hLayout->addWidget(vbox);

    setEnabled(false);
    setFocusPolicy(Qt::NoFocus);

    // disable the two buttons
    m_upButton->setEnabled(false);
    m_downButton->setEnabled(false);

    connect(m_typeCombo, SIGNAL(activated(int)), SLOT(updateData()));
    connect(m_typeCombo, SIGNAL(activated(int)), SLOT(comboBoxChanged(int)));
    connect(m_keyLineEdit, SIGNAL(textChanged(const QString&)), SLOT(updateData()));
    connect(m_valueLineEdit, SIGNAL(textChanged(const QString&)), SLOT(updateData()));
    connect(m_upButton, SIGNAL(clicked()), SIGNAL(moveUp()));
    connect(m_downButton, SIGNAL(clicked()), SIGNAL(moveDown()));
    connect(m_valueLineEdit, SIGNAL(gotFocus()), SLOT(focusInValueHandler()));
    connect(m_valueLineEdit, SIGNAL(lostFocus()), SLOT(focusOutValueHandler()));
    connect(m_typeCombo, SIGNAL(activated(int)), SIGNAL(stateModified()));
    connect(m_keyLineEdit, SIGNAL(textChanged(const QString&)), SIGNAL(stateModified()));
    connect(m_valueLineEdit, SIGNAL(textChanged(const QString&)), SIGNAL(stateModified()));
    connect(m_updatePasswordQualityTimer, SIGNAL(timeout()), SLOT(updateIndicatorLabel()));
    connect(this, SIGNAL(stateModified()), SIGNAL(passwordStrengthUpdated()));
}


/*!
    Clears the south panel.
*/
void SouthPanel::clear()
{
    m_currentProperty = 0;
    m_lastStrength = Property::PUndefined;
    m_indicatorLabel->setPixmap(QPixmap());
    m_indicatorLabel->repaint(true);
    QToolTip::remove(m_indicatorLabel);
    blockSignals(true);
    m_keyLineEdit->setText(QString::null);
    m_valueLineEdit->setText(QString::null);
    m_typeCombo->setCurrentItem(0);
    blockSignals(false);
    m_valueLineEdit->setEchoMode(QLineEdit::Normal);
    m_oldComboValue = -1;
    setEnabled(false);
}


/*!
    Sets item with the property.
*/
void SouthPanel::setItem (Property* property)
{
    clear();

    if (property != 0)
    {
        blockSignals(true);
        m_typeCombo->setCurrentItem(property->getType());
        m_valueLineEdit->setText(property->getValue());
        m_keyLineEdit->setText(property->getKey());
        blockSignals(false);
        m_oldComboValue = property->getType();
        if (property->isHidden())
        {
            m_valueLineEdit->setEchoMode(QLineEdit::Password);
        }
        insertAutoText();
    }
    m_currentProperty = property;
    updateIndicatorLabel(false);

    setEnabled(property != 0);
}


/*!
    Updates the traffic light.
    \param recompute whether recomputation of the password strength should take place
*/
void SouthPanel::updateIndicatorLabel(bool recompute)
{
    if (m_currentProperty && m_currentProperty->getType() == Property::PASSWORD)
    {
        try
        {
            if (recompute)
            {
                m_currentProperty->updatePasswordStrength();
            }
            if (m_currentProperty->getPasswordStrength() != m_lastStrength)
            {
                emit passwordStrengthUpdated();
                m_lastStrength = m_currentProperty->getPasswordStrength();
                switch (m_lastStrength)
                {
                    case Property::PWeak:
                        m_indicatorLabel->setPixmap(QPixmap(":/images/traffic_red_22.png"));
                        break;
                    case Property::PAcceptable:
                        m_indicatorLabel->setPixmap(QPixmap(":/images/traffic_yellow_22.png"));
                        break;
                    case Property::PStrong:
                        m_indicatorLabel->setPixmap(QPixmap(":/images/traffic_green_22.png"));
                        break;
                    case Property::PUndefined:
                        m_indicatorLabel->setPixmap(QPixmap(":/images/traffic_out_22.png"));
                        break;
                }
            }

            QString timeString = StringDisplay::displayTimeSuitable(m_currentProperty->daysToCrack());
            QString tooltipString = QString("Crack time: %1").arg(timeString);
            QToolTip::add(m_indicatorLabel, tooltipString);
        }
        catch (const PasswordCheckException& e)
        {
            // possible workaround to prevent multiple selection in the list this time
            // A bit unclean ???
            /* QMessageBox* mb = new QMessageBox("QPaMaT", tr("<qt><nobr>Failed to calculate the password "
                "strength. The error message</nobr> was:<p>%1<p>Check your configuration!</qt>")
                .arg(e.what()), QMessageBox::Warning, QMessageBox::Ok, QMessageBox::NoButton,
                QMessageBox::NoButton, this, 0, true, WDestructiveClose);
            mb->show(); */
            QMessageBox::warning(this, "QPaMaT", tr("<qt><nobr>Failed to calculate the password "
                "strength. The error message</nobr> was:<p>%1<p>Check your configuration!</qt>")
                .arg(e.what()), QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
    else
    {
        m_indicatorLabel->setPixmap(QPixmap());
        m_indicatorLabel->repaint(true);
        QToolTip::remove(m_indicatorLabel);
    }
}


/*!
    Updates the data.
*/
void SouthPanel::updateData()
{
    if (m_currentProperty != 0)
    {
        m_currentProperty->setType(Property::Type(m_typeCombo->currentItem()));
        m_currentProperty->setHidden(m_typeCombo->currentItem() == Property::PASSWORD);
        m_currentProperty->setEncrypted(m_typeCombo->currentItem() == Property::PASSWORD);
        m_currentProperty->setValue(m_valueLineEdit->text());
        m_updatePasswordQualityTimer->start(200, true);
        m_currentProperty->setKey(m_keyLineEdit->text());
        m_valueLineEdit->setEchoMode(
            m_currentProperty->isHidden()
                ? QLineEdit::Password
                : QLineEdit::Normal );
    }
}


/*!
    Choice of the combo changed.
*/
void SouthPanel::comboBoxChanged(int newChoice)
{

    if (m_oldComboValue != newChoice)
    {
        insertAutoText();

        if (m_oldComboValue == Property::PASSWORD)
        {
            m_currentProperty->setValue("");
            m_valueLineEdit->setText("");
        }

        updateIndicatorLabel(false);
    }
    m_oldComboValue = newChoice;
}


/*!
    Inserts the appropriate AutoText.
*/
void SouthPanel::insertAutoText()
{
    if (m_keyLineEdit->text().isEmpty())
    {
        QString newTxt;
        switch ( Property::Type(m_typeCombo->currentItem()) )
        {
            case Property::MISC:
                newTxt = qpamat->set().readEntry("AutoText/Misc");
                break;
            case Property::USERNAME:
                newTxt = qpamat->set().readEntry("AutoText/Username");
                break;
            case Property::PASSWORD:
                newTxt = qpamat->set().readEntry("AutoText/Password");
                break;
            case Property::URL:
                newTxt = qpamat->set().readEntry("AutoText/URL");
                break;
            default:
                PRINT_DBG("Value is out of range: %d", m_typeCombo->currentItem() );
                break;
        }
        m_keyLineEdit->setText(newTxt);
        m_valueLineEdit->setFocus();
    }
}


/*!
    Returns if the focus is inside this object.
    \return \c true if the focus is inside this object, \c false otherwise.
*/
bool SouthPanel::isFocusInside() const
{
    return m_keyLineEdit->hasFocus() || m_typeCombo->hasFocus() || m_valueLineEdit->hasFocus();
}


/*!
    Enables or disables the moving buttons.
    \param up \c true if the up button should be enabled, \c false otherwise
    \param down \c true if the down button should be enabled, \c false otherwise
*/
void SouthPanel::setMovingEnabled(bool up, bool down)
{
    m_upButton->setEnabled(up);
    m_downButton->setEnabled(down);
}


/*!
    Handles the event if the password edit got the focus.
    Emits the passwordLineEditGotFocus signal.
*/
void SouthPanel::focusInValueHandler()
{
    if (m_currentProperty && m_currentProperty->getType() == Property::PASSWORD)
    {
        passwordLineEditGotFocus(true);
    }
}


/*!
    Handles the event if the password lost the focus. Emits the passwordLineEditGotFocus signal.
*/
void SouthPanel::focusOutValueHandler()
{
    if (m_currentProperty && m_currentProperty->getType() == Property::PASSWORD)
    {
        emit passwordLineEditGotFocus(false);
    }
}


/*!
    Inserts a password in the password edit.
    \param password the password
*/
void SouthPanel::insertPassword(const QString& password)
{
    m_valueLineEdit->insert(password);
}

// :maxLineLen=100:
