/*
 * Id: $Id: configurationdialog.cpp,v 1.1 2003/11/16 20:22:18 bwalle Exp $
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
#include <qtabdialog.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qcursor.h>

#include "configurationdialog.h"
#include "../settings.h"
#include "../cipher/encryptor.h"
#include "../smartcard/memorycard.h"

using ConfigurationDialogLocal::GeneralTab;
using ConfigurationDialogLocal::SmartcardTab;

// -------------------------------------------------------------------------------------------------
ConfigurationDialog::ConfigurationDialog(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QTabDialog(parent)
{
    setCaption("QPaMaT");
    
    m_generalTab = new GeneralTab(this);
    m_smartCardTab = new SmartcardTab(this);
    
    addTab(m_generalTab, "&General");
    addTab(m_smartCardTab, "Smart &Card");
    
    setCancelButton(tr("Cancel"));
    connect(this, SIGNAL(applyButtonPressed()), m_generalTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_smartCardTab, SLOT(applySettings()));
}


// -------------------------------------------------------------------------------------------------
GeneralTab::GeneralTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    QGroupBox* locationsGroup = new QGroupBox(4, Vertical, tr("Locations"), this);
    QGroupBox* encryptionGroup = new QGroupBox(2, Vertical, tr("Encryption"), this);
    locationsGroup->setInsideSpacing(6);
    encryptionGroup->setInsideSpacing(6);
    mainLayout->addWidget(locationsGroup);
    mainLayout->addWidget(encryptionGroup);
    mainLayout->addStretch(5);
    
    // labels  & edit fields
    QLabel* browserLabel = new QLabel(tr("&Web Browser (full path if not in PATH environment):"), 
        locationsGroup);
    m_browserEdit = new FileLineEdit(locationsGroup);
    
    QLabel* datafileLabel = new QLabel(tr("&Data File:"), locationsGroup);
    m_datafileEdit = new FileLineEdit(locationsGroup);
    
    QLabel* algorithmLabel = new QLabel(tr("&Algorithm (don't change this if the names don't "
        "tell you anything):"), encryptionGroup);
    m_algorithmCombo = new QComboBox(false, encryptionGroup);
    
    // set buddys
    browserLabel->setBuddy(m_browserEdit);
    datafileLabel->setBuddy(m_datafileEdit);
    algorithmLabel->setBuddy(m_algorithmCombo);
    
    // minimum width
    //m_browserEdit->setMinimumWidth(450);
    
    locationsGroup->setFlat(true);
    encryptionGroup->setFlat(true);
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    m_algorithmCombo->insertStringList(Encryptor::getAlgorithms());
    QSettings& set = Settings::getInstance().getSettings();
    
    m_browserEdit->setContent( set.readEntry("General/Webbrowser", Settings::DEFAULT_WEBBROWSER) );
    m_datafileEdit->setContent( set.readEntry("General/Datafile", Settings::QPAMAT_FILE_NAME) );
    m_algorithmCombo->setCurrentText( set.readEntry( "General/CipherAlgorithm",
        Encryptor::getSuggestedAlgorithm()) );
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    set.writeEntry( "General/Webbrowser", m_browserEdit->getContent() );
    set.writeEntry( "General/Datafile", m_datafileEdit->getContent() );
    set.writeEntry( "General/CipherAlgorithm", m_algorithmCombo->currentText() );
}


// -------------------------------------------------------------------------------------------------
SmartcardTab::SmartcardTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
    connect(m_radioGroup, SIGNAL(clicked(int)), this, SLOT(radioButtonHandler(int)));
    connect(m_testButton, SIGNAL(clicked()), this, SLOT(testSmartCard()));
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    
    
    m_settingsGroup = new QGroupBox(4, Vertical, tr("Settings"), this);
    m_radioGroup = new QButtonGroup(2, Vertical, QString::null, this);
    m_testGroup = new QButtonGroup(2, Vertical, tr("Testing"), this);
    
    m_settingsGroup->setFlat(true);
    m_testGroup->setFlat(true);
    m_radioGroup->setFrameStyle(QFrame::Plain);
    
    new QRadioButton(tr("&Don't use a smartcard"), m_radioGroup);
    new QRadioButton(tr("&Use a smartcard"), m_radioGroup);
    
    QLabel* libraryLabel = new QLabel(tr("&CT-API Chipcard Driver:"), m_settingsGroup);
    m_libraryEdit = new FileLineEdit(m_settingsGroup);
    
    QLabel* portLabel = new QLabel(tr("Chipcard Terminal &Port:"), m_settingsGroup);
    m_portCombo = new QComboBox(false, m_settingsGroup);
    
    new QLabel(tr("Insert a card and click on this button to test your settings:"), m_testGroup);
    QHBox* box = new QHBox(m_testGroup);
    m_testButton = new QPushButton(tr("&Test"), box);
    m_testButton->setAutoDefault(false);
    
    QWidget* dummy = new QWidget(box);
    box->setStretchFactor(dummy, 10);
    
    mainLayout->addWidget(m_radioGroup);
    mainLayout->addWidget(m_settingsGroup);
    mainLayout->addWidget(m_testGroup);
    mainLayout->addStretch(5);
    
    
    // the case discrimination if rom Qt's code, so it should be correct
#if defined Q_WS_WIN
    m_libraryEdit->setFilter(tr("CT-API-Driver (*.dll)"));
#elif Q_OS_MACX
    m_libraryEdit->setFilter(tr("CT-API-Driver (*.dylib)"));
#elif defined(Q_OS_HPUX)
    m_libraryEdit->setFilter(tr("CT-API-Driver (*.sl)"));
#else
    m_libraryEdit->setFilter(tr("CT-API-Driver (*.so)"));
#endif
    
    // set buddys
    libraryLabel->setBuddy(m_libraryEdit);
    portLabel->setBuddy(m_portCombo);
    
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::radioButtonHandler(int buttonId)
// -------------------------------------------------------------------------------------------------
{
    setUseSmartcardEnabled( SmartcardEnabled(buttonId) == Enabled );
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::setUseSmartcardEnabled(bool enabled)
// -------------------------------------------------------------------------------------------------
{
    m_settingsGroup->setEnabled(enabled);
    m_testGroup->setEnabled(enabled);
    m_radioGroup->setButton( enabled ? Enabled : NotEnabled );
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    m_portCombo->clear();
    const char* ports[] = { QT_TR_NOOP("Special (Port 0)"), QT_TR_NOOP("COM1/USB/Keyboard"), 
        QT_TR_NOOP("COM2"), QT_TR_NOOP("COM3"), QT_TR_NOOP("COM4") };
    m_portCombo->insertStrList(ports, 5);
    
    QSettings& set = Settings::getInstance().getSettings();
    
    m_libraryEdit->setContent( set.readEntry("Smartcard/Library", "" ));
    m_portCombo->setCurrentItem( set.readNumEntry("Smartcard/Port", Settings::DEFAULT_PORT) );
    setUseSmartcardEnabled( set.readBoolEntry("Smartcard/UseCard", Settings::DEFAULT_USE_CARD ));
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    set.writeEntry( "Smartcard/Library", m_libraryEdit->getContent() );
    set.writeEntry( "Smartcard/Port", m_portCombo->currentItem() );
    set.writeEntry( "Smartcard/UseCard", SmartcardEnabled(m_radioGroup->selectedId()) == Enabled);
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::testSmartCard()
// -------------------------------------------------------------------------------------------------
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    try
    {
        MemoryCard card(m_libraryEdit->getContent());
        card.init(m_portCombo->currentItem());
        MemoryCard::CardType type = card.getType();
        
        if (type == MemoryCard::TMemoryCard)
        {
            int capacity = 0;
            card.resetCard(&capacity);
            
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "QPaMaT", QString("<qt>"+tr(
                "<p>Detected a card with %1 kBytes memory.</p><p>If you think this is "
                "enough memory you can use this card for storing your passwords!</p>")+"</qt>").arg(
                QString::number(capacity / 1024.0)),
                QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        }
        else
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(this, "QPaMaT", QString("<qt>"+tr(
                "<p>The communication to your chipcard terminal seems to work.<p>"
                "<p>However, there's no memory card in your reader. So you cannot use "
                "it for saving your password. Buy a memory card and try again!</p>")),
                QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
        }
        
        // if we don't access this point the destructor closes it. So no problem here!
        card.close();
    }
    catch (const NoSuchLibraryException& ex)
    {
        QApplication::restoreOverrideCursor();
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", QString("<qt>"+tr(
                "<p>A problem occured while loading the specified CT-API (chipcard) driver."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
    catch (const CardException& ex)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", QString("<qt>"+tr(
                "<p>An error occured while communicating with the chipcard terminal."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
}
