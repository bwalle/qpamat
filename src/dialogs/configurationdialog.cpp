/*
 * Id: $Id: configurationdialog.cpp,v 1.11 2003/12/20 15:58:12 bwalle Exp $
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
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qmessagebox.h>
#include <qaction.h>
#include <qapplication.h>
#include <qcursor.h>

#include "../qpamat.h"
#include "configurationdialog.h"
#include "../security/passwordgeneratorfactory.h"
#include "../security/symmetricencryptor.h"
#include "../smartcard/memorycard.h"


using ConfigurationDialogLocal::GeneralTab;
using ConfigurationDialogLocal::SmartcardTab;
using ConfigurationDialogLocal::SecurityTab;
using ConfigurationDialogLocal::PresentationTab;

// -------------------------------------------------------------------------------------------------
ConfigurationDialog::ConfigurationDialog(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QTabDialog(parent), m_generalTab(0), m_securityTab(0), m_smartCardTab(0), m_printingTab(0)
{
    setCaption("QPaMaT");
    
    m_generalTab = new GeneralTab(this);
    m_smartCardTab = new SmartcardTab(this);
    m_securityTab = new SecurityTab(this);
    m_printingTab = new PresentationTab(this);
    
    addTab(m_generalTab, "&General");
    addTab(m_securityTab, "&Security"); 
    addTab(m_smartCardTab, "Smart &Card");
    addTab(m_printingTab, "&Presentation");
    
    setCancelButton(tr("Cancel"));
    connect(this, SIGNAL(applyButtonPressed()), m_generalTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_securityTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_smartCardTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), m_printingTab, SLOT(applySettings()));
    
    QAction* whatsThis = new QAction("What's this", QKeySequence(Key_F1), this);
    connect(whatsThis, SIGNAL(activated()), qpamat, SLOT(whatsThis()));
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
    m_autoLoginCheckbox->setChecked(qpamat->set().readBoolEntry("General/AutoLogin"));
    m_browserEdit->setContent(qpamat->set().readEntry("General/Webbrowser"));
    m_datafileEdit->setContent(qpamat->set().readEntry("General/Datafile"));
    m_miscEdit->setText(qpamat->set().readEntry("AutoText/Misc"));
    m_usernameEdit->setText(qpamat->set().readEntry("AutoText/Username"));
    m_passwordEdit->setText(qpamat->set().readEntry("AutoText/Password"));
    m_urlEdit->setText(qpamat->set().readEntry("AutoText/URL"));
}


// -------------------------------------------------------------------------------------------------
void GeneralTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    qpamat->set().writeEntry("General/AutoLogin", m_autoLoginCheckbox->isChecked() );
    qpamat->set().writeEntry("General/Webbrowser", m_browserEdit->getContent() );
    qpamat->set().writeEntry("General/Datafile", m_datafileEdit->getContent() );
    qpamat->set().writeEntry("AutoText/Misc", m_miscEdit->text() );
    qpamat->set().writeEntry("AutoText/Username", m_usernameEdit->text() );
    qpamat->set().writeEntry("AutoText/Password", m_passwordEdit->text() );
    qpamat->set().writeEntry("AutoText/URL", m_urlEdit->text() );
}


// -------------------------------------------------------------------------------------------------
SecurityTab::SecurityTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent), m_lengthSpinner(0), m_externalEdit(0), m_allowedCharsEdit(0), 
          m_useExternalCB(0), m_uppercaseCB(0), m_lowercaseCB(0), m_digitsCB(0), m_specialCB(0), 
          m_algorithmCombo(0)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
    connect(m_useExternalCB, SIGNAL(toggled(bool)), SLOT(checkboxHandler(bool)));
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    QGroupBox* passwordGroup = new QGroupBox(5, Vertical, tr("Generated Passwords"), this);
    QGroupBox* encryptionGroup = new QGroupBox(2, Vertical, tr("Encryption"), this);
    
    // some settings
    encryptionGroup->setInsideSpacing(6);  // ???
    passwordGroup->setFlat(true);
    passwordGroup->setInsideSpacing(6);
    encryptionGroup->setFlat(true);
    
    QWidget* ensureGrid = new QWidget(passwordGroup, "EnsureGrid");
    QGridLayout* ensureGridLayout = new QGridLayout(ensureGrid, 5, 3, 0, 6, "EnsureGridLayout");
    QLabel* ensureLabel = new QLabel(tr("Ensure following components of a password:"), 
        ensureGrid, "EnsureLabel");
    m_uppercaseCB = new QCheckBox(tr("&Uppercase letters"), ensureGrid);
    m_lowercaseCB = new QCheckBox(tr("&Lowercase letters"), ensureGrid);
    m_digitsCB    = new QCheckBox(tr("&Digits"), ensureGrid);
    m_specialCB   = new QCheckBox(tr("&Special characters"), ensureGrid);
    QLabel* lengthLabel = new QLabel(tr("L&ength:"), ensureGrid);
    QLabel* allowedLabel = new QLabel(tr("&Allowed characters:"), ensureGrid);
    m_lengthSpinner = new QSpinBox(6, 20, 1, ensureGrid, "LengthSpinner");
    m_allowedCharsEdit = new QLineEdit(ensureGrid, "AllowedLineEdit");
    
    // layout the grid
    ensureGridLayout->setColSpacing(1, 6);
    ensureGridLayout->addMultiCellWidget(ensureLabel, 0, 0, 0, 2);
    ensureGridLayout->addWidget(m_uppercaseCB,        1, 0);
    ensureGridLayout->addWidget(m_lowercaseCB,        1, 2);
    ensureGridLayout->addWidget(m_digitsCB,           2, 0);
    ensureGridLayout->addWidget(m_specialCB,          2, 2);
    ensureGridLayout->addWidget(lengthLabel,          3, 0);
    ensureGridLayout->addWidget(allowedLabel,         3, 2);
    ensureGridLayout->addWidget(m_lengthSpinner,      4, 0);
    ensureGridLayout->addWidget(m_allowedCharsEdit,   4, 2);
    
    m_useExternalCB = new QCheckBox(tr("Use external application for generation:"), passwordGroup,
        "ExternalCB");
    m_externalEdit = new FileLineEdit(passwordGroup, "ExternalEdit");
    
    // algorithm stuff
    QLabel* algorithmLabel = new QLabel(tr("Cipher &algorithm:"), encryptionGroup);
    m_algorithmCombo = new QComboBox(false, encryptionGroup); 
    
    // buddys
    algorithmLabel->setBuddy(m_algorithmCombo);
    lengthLabel->setBuddy(m_lengthSpinner);
    allowedLabel->setBuddy(m_allowedCharsEdit);
    
    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(encryptionGroup);
    mainLayout->addSpacing(10);
    mainLayout->addStretch(5);
    
    // help
    /* QWhatsThis::add(m_radioGroup, tr("<qt>Password checkers are used to check the initial password "
        "and the random password. That means that a random password has always the security of the "
        "password checker.<p>Using an external password checker may be slow.</qt>")); */
    QWhatsThis::add(m_algorithmCombo, tr("<qt>Only change this if you know what you do. The "
        "algorithms are provided by the OpenSSL library and the availability is determined "
        "at runtime. You can read a file encrypted with <i>X</i> only if the computer on which "
        "you read it is able to handle algorithm <i>X</i>. <p>Blowfish is a good choise because "
        "it's free and available everywhere. IDEA is patended (but secure, PGP uses it!) "
        "and AES (the successor of DES) is only available at new versions of OpenSSL."
        "Read a book about cryptography if you're interested in this algorithms.</qt>"));
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::checkboxHandler(bool on)
// -------------------------------------------------------------------------------------------------
{
    m_externalEdit->setEnabled(on);
    if (!on)
    {
        m_externalEdit->clearFocus();
    }
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    QString ensuredString = qpamat->set().readEntry("Security/EnsuredCharacters");
    
    m_uppercaseCB->setChecked(ensuredString.contains("U"));
    m_lowercaseCB->setChecked(ensuredString.contains("L"));
    m_specialCB->setChecked(ensuredString.contains("S"));
    m_digitsCB->setChecked(ensuredString.contains("D"));
    m_lengthSpinner->setValue(qpamat->set().readNumEntry("Security/Length"));
    m_allowedCharsEdit->setText(qpamat->set().readEntry("Security/AllowedCharacters"));
    m_useExternalCB->setChecked(qpamat->set().readEntry("Security/PasswordGenerator") =="EXTERNAL");
    m_externalEdit->setContent(qpamat->set().readEntry("Security/PasswordGenAdditional"));
    
    checkboxHandler(m_useExternalCB->isChecked());
    
    m_algorithmCombo->insertStringList(SymmetricEncryptor::getAlgorithms());
    m_algorithmCombo->setCurrentText( qpamat->set().readEntry( "Security/CipherAlgorithm" ));
}


// -------------------------------------------------------------------------------------------------
void SecurityTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    QString ensuredString;
    ensuredString += m_uppercaseCB->isChecked() ? "U" : "u";
    ensuredString += m_lowercaseCB->isChecked() ? "L" : "l";
    ensuredString += m_specialCB->isChecked()   ? "S" : "s";
    ensuredString += m_digitsCB->isChecked()    ? "D" : "d";
    
    qpamat->set().writeEntry("Security/PasswordGenerator", m_useExternalCB->isChecked() 
        ? "EXTERNAL" : "RANDOM" );
    qpamat->set().writeEntry("Security/PasswordGenAdditional", m_externalEdit->getContent());
    qpamat->set().writeEntry("Security/EnsuredCharacters", ensuredString);
    qpamat->set().writeEntry("Security/Length", m_lengthSpinner->value());
    qpamat->set().writeEntry("Security/AllowedCharacters", m_allowedCharsEdit->text());
    qpamat->set().writeEntry("Security/CipherAlgorithm", m_algorithmCombo->currentText() );
}


// -------------------------------------------------------------------------------------------------
PresentationTab::PresentationTab(QWidget* parent)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
}


// -------------------------------------------------------------------------------------------------
void PresentationTab::createAndLayout()
// -------------------------------------------------------------------------------------------------
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    QGroupBox* passwordGroup = new QGroupBox(1, Vertical, tr("Passwords"), this);
    QGroupBox* fontGroup = new QGroupBox(4, Vertical, tr("Printing Fonts"), this);
     
    
    // some settings
    passwordGroup->setInsideSpacing(6);
    passwordGroup->setFlat(true);
    fontGroup->setInsideSpacing(6);
    fontGroup->setFlat(true);
    
    QLabel* normalLabel = new QLabel("&Normal font:", fontGroup);
    m_normalFontEdit = new FontChooseBox(fontGroup);
    QLabel* footerLabel = new QLabel("&Footer font:", fontGroup);
    m_footerFontEdit = new FontChooseBox(fontGroup);
    
    m_hidePasswordCB = new QCheckBox(tr("Hide passwords in Random Password Dialog"), 
        passwordGroup, "HidePasswords");
    
    // buddys
    normalLabel->setBuddy(m_normalFontEdit);
    footerLabel->setBuddy(m_footerFontEdit);
    
    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(fontGroup);
    mainLayout->addStretch(5);
    
}


// -------------------------------------------------------------------------------------------------
void PresentationTab::fillSettings()
// -------------------------------------------------------------------------------------------------
{
    QFont font;
    
    font.fromString(qpamat->set().readEntry( "Presentation/NormalFont"));
    m_normalFontEdit->setFont(font);
    font.fromString(qpamat->set().readEntry( "Presentation/FooterFont"));
    m_footerFontEdit->setFont(font);
    m_hidePasswordCB->setChecked(qpamat->set().readBoolEntry("Presentation/HideRandomPass"));
}


// -------------------------------------------------------------------------------------------------
void PresentationTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    qpamat->set().writeEntry("Presentation/HideRandomPass", m_hidePasswordCB->isChecked());
    qpamat->set().writeEntry("Presentation/NormalFont", m_normalFontEdit->getFont().toString());
    qpamat->set().writeEntry("Presentation/FooterFont", m_footerFontEdit->getFont().toString());
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
    
    m_libraryEdit->setContent(qpamat->set().readEntry("Smartcard/Library"));
    m_portCombo->setCurrentItem(qpamat->set().readNumEntry("Smartcard/Port"));
    setUseSmartcardEnabled(qpamat->set().readBoolEntry("Smartcard/UseCard"));
}


// -------------------------------------------------------------------------------------------------
void SmartcardTab::applySettings()
// -------------------------------------------------------------------------------------------------
{
    qpamat->set().writeEntry("Smartcard/Library", m_libraryEdit->getContent() );
    qpamat->set().writeEntry("Smartcard/Port", m_portCombo->currentItem() );
    qpamat->set().writeEntry("Smartcard/UseCard", 
        SmartcardEnabled(m_radioGroup->selectedId()) == Enabled);
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

