/*
 * Id: $Id: configurationdialog.cpp,v 1.7 2003/12/10 21:43:47 bwalle Exp $
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
#include "../security/passwordcheckerfactory.h"
#include "../security/symmetricencryptor.h"
#include "../smartcard/memorycard.h"


using ConfigurationDialogLocal::GeneralTab;
using ConfigurationDialogLocal::SmartcardTab;
using ConfigurationDialogLocal::SecurityTab;
using ConfigurationDialogLocal::PrintingTab;

// -------------------------------------------------------------------------------------------------
ConfigurationDialog::ConfigurationDialog(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QTabDialog(parent)
{
    setCaption("QPaMaT");
    
    m_generalTab = new GeneralTab(this);
    m_smartCardTab = new SmartcardTab(this);
    m_securityTab = new SecurityTab(this);
    m_printingTab = new PrintingTab(this);
    
    addTab(m_generalTab, "&General");
    addTab(m_securityTab, "&Security");
    addTab(m_smartCardTab, "Smart &Card");
    addTab(m_printingTab, "&Printing");
    
    setCancelButton(tr("Cancel"));
    connect(this, SIGNAL(applyButtonPressed()), m_generalTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_securityTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_smartCardTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_printingTab, SLOT(applySettings()));
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
    QGroupBox* startupGroup = new QGroupBox(1, Vertical, tr("Startup"), this);
    QGroupBox* locationsGroup = new QGroupBox(4, Vertical, tr("Locations"), this);
    QGroupBox* autoTextGroup = new QGroupBox(4, Vertical, tr("AutoText"), this);
    
    // some settings
    startupGroup->setInsideSpacing(6);
    locationsGroup->setInsideSpacing(6);
    autoTextGroup->setInsideSpacing(6);
    startupGroup->setFlat(true);
    locationsGroup->setFlat(true);
    autoTextGroup->setFlat(true);
    
    // auto login
    m_autoLoginCheckbox = new QCheckBox(tr("Enable &AutoLogin on startup"), startupGroup);
    
    // labels  & edit fields
    QLabel* browserLabel = new QLabel(tr("&Web Browser (full path if not in PATH environment):"), 
        locationsGroup);
    m_browserEdit = new FileLineEdit(locationsGroup);
    
    QLabel* datafileLabel = new QLabel(tr("&Data File:"), locationsGroup);
    m_datafileEdit = new FileLineEdit(locationsGroup);
    
    // auto text
    QLabel* miscLabel = new QLabel(tr("&Misc"), autoTextGroup);
    m_miscEdit = new QLineEdit(autoTextGroup);
    QLabel* passwordLabel = new QLabel(tr("&Password"), autoTextGroup);
    m_passwordEdit = new QLineEdit(autoTextGroup);
    QLabel* usernameLabel = new QLabel(tr("&Username"), autoTextGroup);
    m_usernameEdit = new QLineEdit(autoTextGroup);
    QLabel* urlLabel = new QLabel(tr("&URL"), autoTextGroup);
    m_urlEdit = new QLineEdit(autoTextGroup);
    
    // set buddys
    browserLabel->setBuddy(m_browserEdit);
    datafileLabel->setBuddy(m_datafileEdit);
    miscLabel->setBuddy(m_miscEdit);
    usernameLabel->setBuddy(m_usernameEdit);
    passwordLabel->setBuddy(m_passwordEdit);
    urlLabel->setBuddy(m_urlEdit);
    
    mainLayout->addWidget(startupGroup);
    mainLayout->addWidget(locationsGroup);
    mainLayout->addWidget(autoTextGroup);
    mainLayout->addStretch(5);
    
    QWidget::setTabOrder(m_miscEdit, m_usernameEdit);
    QWidget::setTabOrder(m_usernameEdit, m_passwordEdit);
    QWidget::setTabOrder(m_passwordEdit, m_urlEdit);
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    
    m_autoLoginCheckbox->setChecked( set.readBoolEntry("General/AutoLogin", false));
    m_browserEdit->setContent( set.readEntry("General/Webbrowser", Settings::DEFAULT_WEBBROWSER) );
    m_datafileEdit->setContent( set.readEntry("General/Datafile", Settings::QPAMAT_FILE_NAME) );
    m_miscEdit->setText( set.readEntry("AutoText/Misc", Settings::DEFAULT_AUTOTEXT_MISC) );
    m_usernameEdit->setText(set.readEntry("AutoText/Username",Settings::DEFAULT_AUTOTEXT_USERNAME));
    m_passwordEdit->setText(set.readEntry("AutoText/Password",Settings::DEFAULT_AUTOTEXT_PASSWORD));
    m_urlEdit->setText( set.readEntry("AutoText/URL", Settings::DEFAULT_AUTOTEXT_URL) );
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    set.writeEntry( "General/AutoLogin", m_autoLoginCheckbox->isChecked() );
    set.writeEntry( "General/Webbrowser", m_browserEdit->getContent() );
    set.writeEntry( "General/Datafile", m_datafileEdit->getContent() );
    set.writeEntry( "AutoText/Misc", m_miscEdit->text() );
    set.writeEntry( "AutoText/Username", m_usernameEdit->text() );
    set.writeEntry( "AutoText/Password", m_passwordEdit->text() );
    set.writeEntry( "AutoText/URL", m_urlEdit->text() );
}


// -------------------------------------------------------------------------------------------------
SecurityTab::SecurityTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
    connect(m_radioGroup, SIGNAL(clicked(int)), this, SLOT(radioButtonHandler(int)));
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    m_radioGroup = new QButtonGroup(6, Vertical,tr("Password Checker"), this);
    QGroupBox* encryptionGroup = new QGroupBox(2, Vertical, tr("Encryption"), this);
    
    // some settings
    encryptionGroup->setInsideSpacing(6);
    m_radioGroup->setFlat(true);
    encryptionGroup->setFlat(true);
    
    new QRadioButton(tr("S&imple"), m_radioGroup);
    new QRadioButton(tr("&Extended"), m_radioGroup);
    new QRadioButton(tr("&Using dictionary:"), m_radioGroup);
    QWidget* box1 = new QWidget(m_radioGroup);
    QHBoxLayout* hlayout1 = new QHBoxLayout(box1);
    m_dictionaryEdit = new FileLineEdit(box1);
    hlayout1->addSpacing(20);
    hlayout1->addWidget(m_dictionaryEdit);
    
    new QRadioButton(tr("E&xternal Application (full path if not in PATH environment):"), 
        m_radioGroup);
    QWidget* box2 = new QWidget(m_radioGroup);
    QHBoxLayout* hlayout2 = new QHBoxLayout(box2);
    m_externalEdit = new FileLineEdit(box2);
    hlayout2->addSpacing(20);
    hlayout2->addWidget(m_externalEdit);
    
    // algorithm stuff
    QLabel* algorithmLabel = new QLabel(tr("&Algorithm (don't change this if the names don't "
        "tell you anything):"), encryptionGroup);
    m_algorithmCombo = new QComboBox(false, encryptionGroup);
    
    // buddys
    algorithmLabel->setBuddy(m_algorithmCombo);
    
    mainLayout->addWidget(m_radioGroup);
    mainLayout->addWidget(encryptionGroup);
    mainLayout->addStretch(5);
    
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::radioButtonHandler(int buttonId)
// -------------------------------------------------------------------------------------------------
{
    m_dictionaryEdit->setEnabled( PasswordCheckerFactory::PasswordCheckerType(buttonId) == 
        PasswordCheckerFactory::TExtendedDictPasswordChecker );
    m_externalEdit->setEnabled( PasswordCheckerFactory::PasswordCheckerType(buttonId) == 
        PasswordCheckerFactory::TExternalPasswordChecker );
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    
    m_algorithmCombo->insertStringList(SymmetricEncryptor::getAlgorithms());
    m_radioGroup->setButton( PasswordCheckerFactory::fromString( 
        set.readEntry( "Security/PasswordChecker", PasswordCheckerFactory::DEFAULT_CHECKER_STRING))
        );
    m_dictionaryEdit->setContent(set.readEntry( "Security/DictionaryFile"));
    m_externalEdit->setContent(set.readEntry( "Security/ExternalProgram"));
    radioButtonHandler(m_radioGroup->selectedId());
    m_algorithmCombo->setCurrentText( set.readEntry( "Security/CipherAlgorithm",
        SymmetricEncryptor::getSuggestedAlgorithm()) );
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    PasswordCheckerFactory::PasswordCheckerType type = 
        PasswordCheckerFactory::PasswordCheckerType(m_radioGroup->selectedId());
    
    QString additional;
    switch (type)
    {
        case PasswordCheckerFactory::TExtendedDictPasswordChecker:
            additional = m_dictionaryEdit->getContent();
            break;
        case PasswordCheckerFactory::TExternalPasswordChecker:
            additional = m_externalEdit->getContent();
            break;
        default:
            additional = "";
            break;
    }
    
    set.writeEntry("Security/PasswordChecker", PasswordCheckerFactory::toString(type));
    set.writeEntry("Security/PasswordCheckerAdditional", additional);
    set.writeEntry("Security/DictionaryFile", m_dictionaryEdit->getContent() );
    set.writeEntry("Security/ExternalProgram", m_externalEdit->getContent() );
    set.writeEntry("Security/CipherAlgorithm", m_algorithmCombo->currentText() );
}


// -------------------------------------------------------------------------------------------------
PrintingTab::PrintingTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
}


// -------------------------------------------------------------------------------------------------
void PrintingTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    QGroupBox* fontGroup = new QGroupBox(4, Vertical, tr("Fonts"), this);
    
    // some settings
    fontGroup->setInsideSpacing(6);
    fontGroup->setFlat(true);
    
    QLabel* normalLabel = new QLabel("&Normal font:", fontGroup);
    m_normalFontEdit = new FontChooseBox(fontGroup);
    QLabel* footerLabel = new QLabel("&Footer font:", fontGroup);
    m_footerFontEdit = new FontChooseBox(fontGroup);
    
    // buddys
    normalLabel->setBuddy(m_normalFontEdit);
    footerLabel->setBuddy(m_footerFontEdit);
    
    mainLayout->addWidget(fontGroup);
    mainLayout->addStretch(5);
    
}


// -------------------------------------------------------------------------------------------------
void PrintingTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    QFont font;
    
    font.fromString(set.readEntry( "Printing/NormalFont", Settings::DEFAULT_NORMAL_FONT));
    m_normalFontEdit->setFont(font);
    font.fromString(set.readEntry( "Printing/FooterFont", Settings::DEFAULT_FOOTER_FONT));
    m_footerFontEdit->setFont(font);
}


// -------------------------------------------------------------------------------------------------
void PrintingTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    
    set.writeEntry("Printing/NormalFont", m_normalFontEdit->getFont().toString());
    set.writeEntry("Printing/FooterFont", m_footerFontEdit->getFont().toString());
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
    
    QLabel* libraryLabel = new QLabel(tr("CT-&API Chipcard Driver:"), m_settingsGroup);
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
                "<p>A problem occurred while loading the specified CT-API (chipcard) driver."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
    catch (const CardException& ex)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", QString("<qt>"+tr(
                "<p>An error occurred while communicating with the chipcard terminal."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
}

