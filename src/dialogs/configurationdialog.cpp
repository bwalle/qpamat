/*
 * Id: $Id: configurationdialog.cpp,v 1.13 2003/12/28 22:08:15 bwalle Exp $
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
#include <qslider.h>
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
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlcdnumber.h>

#include "configurationdialog.h"
#include "configurationdialogprivate.h"
#include "qpamat.h"
#include "widgets/filelineedit.h"
#include "security/passwordgeneratorfactory.h"
#include "security/symmetricencryptor.h"
#include "smartcard/memorycard.h"

/*!
    \class ConfigurationDialog
    
    \brief This dialog displays the configuration and lets the user change it.
    
    The whole configuration is done by this dialog. There's not very much to say, the
    dialog has four tabs:
    
     - \b General which describes general display options
     - \b Security which does the security settings
     - \b SmartCard which controls all stuff about smartcards
     - \b Presenstation which holds all about printing and some other things
    
    Just use following to use the dialog:
    
    \code
  std::auto_ptr<ConfigurationDialog> dlg(new ConfigurationDialog(this));
  if (dlg->exec() == QDialog::Accepted)
  {
      // do something
  }
    \endcode
    
    Using std::auto_ptr here is a good idea but not neccessary.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.13 $
    \date $Date: 2003/12/28 22:08:15 $
 */

/*!
    Creates a new instance of a ConfigurationDialog.
*/
ConfigurationDialog::ConfigurationDialog(QWidget* parent)
    : QTabDialog(parent)
{
    setCaption("QPaMaT");
    
    ConfDlgGeneralTab* generalTab = new ConfDlgGeneralTab(this);
    ConfDlgSmartcardTab* smartCardTab = new ConfDlgSmartcardTab(this);
    ConfDlgSecurityTab* securityTab = new ConfDlgSecurityTab(this);
    ConfDlgPresentationTab* presentationTab = new ConfDlgPresentationTab(this);
    
    addTab(generalTab, "&General");
    addTab(securityTab, "&Security"); 
    addTab(smartCardTab, "Smart &Card");
    addTab(presentationTab, "&Presentation");
    
    setCancelButton(tr("Cancel"));
    connect(this, SIGNAL(applyButtonPressed()), generalTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), smartCardTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), securityTab, SLOT(applySettings()));
    connect(this, SIGNAL(applyButtonPressed()), presentationTab, SLOT(applySettings()));
    
    QAction* whatsThis = new QAction("What's this", QKeySequence(Key_F1), this);
    connect(whatsThis, SIGNAL(activated()), qpamat, SLOT(whatsThis()));
    
    adjustSize();
}

#ifndef DOXYGEN

// -------------------------------------------------------------------------------------------------
//                                     General tab
// -------------------------------------------------------------------------------------------------

/*!
    \class ConfDlgGeneralTab
    
    \brief Represents general settings in the configuration dialog
    
    This tab holds all general settings 
    
     - startup (AutoLogin)
     - locations of external applications
     - AutoText function
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.13 $
    \date $Date: 2003/12/28 22:08:15 $
*/


/*!
    Creates a new instance of an ConfDlgGeneralTab object.
    \param parent the parent widget
*/
ConfDlgGeneralTab::ConfDlgGeneralTab(QWidget* parent)
    : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
}


/*!
    Does creating of subwidgets and layout of the GeneralTab.
*/
void ConfDlgGeneralTab::createAndLayout()
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


/*!
    Fills the settings. This method is called from the dialog's constructor.
*/
void ConfDlgGeneralTab::fillSettings()
{
    m_autoLoginCheckbox->setChecked(qpamat->set().readBoolEntry("General/AutoLogin"));
    m_browserEdit->setContent(qpamat->set().readEntry("General/Webbrowser"));
    m_datafileEdit->setContent(qpamat->set().readEntry("General/Datafile"));
    m_miscEdit->setText(qpamat->set().readEntry("AutoText/Misc"));
    m_usernameEdit->setText(qpamat->set().readEntry("AutoText/Username"));
    m_passwordEdit->setText(qpamat->set().readEntry("AutoText/Password"));
    m_urlEdit->setText(qpamat->set().readEntry("AutoText/URL"));
}


/*!
    Applys the settings. This method is called by if the user presses the Ok button.
*/
void ConfDlgGeneralTab::applySettings()
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
//                                     Security tab
// -------------------------------------------------------------------------------------------------

/*!
    \class ConfDlgSecurityTab
    
    \brief Represents security settings in the configuration dialog
    
    This tab holds security general settings 
    
     - settings for generated passwords
     - password strength settings
     - cipher algorithm
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.13 $
    \date $Date: 2003/12/28 22:08:15 $
*/


/*!
    Creates a new instance of an ConfDlgSecurityTab object.
    \param parent the parent widget
*/
ConfDlgSecurityTab::ConfDlgSecurityTab(QWidget* parent)
    : QWidget(parent), m_lengthSpinner(0), m_externalEdit(0), m_allowedCharsEdit(0), 
      m_useExternalCB(0), m_weakSlider(0), m_strongSlider(0), m_weakLabel(0), m_strongLabel(0),
      m_algorithmCombo(0)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
    connect(m_useExternalCB, SIGNAL(toggled(bool)), SLOT(checkboxHandler(bool)));
    connect(m_weakSlider, SIGNAL(valueChanged(int)), SLOT(weakSliderHandler(int)));
    connect(m_strongSlider, SIGNAL(valueChanged(int)), SLOT(strongSliderHandler(int)));
}


/*!
    Does creating of subwidgets and layout of the GeneralTab.
*/
void ConfDlgSecurityTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 6, 6);
    QGroupBox* passwordGroup = new QGroupBox(5, Vertical, tr("Generated Passwords"), this);
    QGroupBox* checkerGroup = new QGroupBox(5, Vertical, tr("Password checker"), this);
    QGroupBox* encryptionGroup = new QGroupBox(1, Vertical, tr("Encryption"), this);
    
    // some settings
    encryptionGroup->setInsideSpacing(12);
    encryptionGroup->setFlat(true);
    passwordGroup->setInsideSpacing(6);
    passwordGroup->setFlat(true);
    checkerGroup->setInsideSpacing(6);
    checkerGroup->setFlat(true);
    
    QWidget* ensureGrid = new QWidget(passwordGroup, "EnsureGrid");
    QGridLayout* ensureGridLayout = new QGridLayout(ensureGrid, 2, 3, 0, 6, "EnsureGridLayout");
    QLabel* lengthLabel = new QLabel(tr("L&ength:"), ensureGrid);
    QLabel* allowedLabel = new QLabel(tr("&Allowed characters:"), ensureGrid);
    m_lengthSpinner = new QSpinBox(6, 20, 1, ensureGrid, "LengthSpinner");
    m_allowedCharsEdit = new QLineEdit(ensureGrid, "AllowedLineEdit");
    
    // layout the grid
    ensureGridLayout->setColStretch(0, 4);
    ensureGridLayout->setColStretch(1, 0);
    ensureGridLayout->setColStretch(2, 5);
    ensureGridLayout->setColSpacing(1, 6);
    ensureGridLayout->addWidget(lengthLabel,          3, 0);
    ensureGridLayout->addWidget(allowedLabel,         3, 2);
    ensureGridLayout->addWidget(m_lengthSpinner,      4, 0);
    ensureGridLayout->addWidget(m_allowedCharsEdit,   4, 2);
    
    m_useExternalCB = new QCheckBox(tr("Use external application for generation:"), passwordGroup,
        "ExternalCB");
    m_externalEdit = new FileLineEdit(passwordGroup, "ExternalEdit");
    
    // checker stuff
    new QLabel(tr("Limits for weak - acceptable - strong (cracking days):"), checkerGroup, "WeakLabel");
    QHBox* weakSliderBox = new QHBox(checkerGroup, "WeakSliderBox");
    weakSliderBox->setSpacing(4);
    m_weakSlider = new QSlider(0, 100, 1, 2, Qt::Horizontal, weakSliderBox, "WeakSlider");
    m_weakLabel = new QLCDNumber(3, weakSliderBox);
    m_weakLabel->setSmallDecimalPoint(true);
    m_weakLabel->setLineWidth(0);
    
    QHBox* strongSliderBox = new QHBox(checkerGroup, "WeakSliderBox");
    strongSliderBox->setSpacing(4);
    m_strongSlider = new QSlider(0, 100, 1, 15, Qt::Horizontal, strongSliderBox, "WeakSlider");
    m_strongLabel = new QLCDNumber(3, strongSliderBox);
    m_strongLabel->setSmallDecimalPoint(true);
    m_strongLabel->setLineWidth(0);
    
    QLabel* dictLabel = new QLabel(tr("&Dictionary file (sorted!):"), checkerGroup, "DictLabel");
    m_dictionaryEdit = new FileLineEdit(checkerGroup, "DictEdit");
    
    // algorithm stuff
    m_algorithmLabel = new QLabel(tr("Cipher &algorithm:"), encryptionGroup);
    m_algorithmCombo = new QComboBox(false, encryptionGroup); 
    
    // buddys
    m_algorithmLabel->setBuddy(m_algorithmCombo);
    lengthLabel->setBuddy(m_lengthSpinner);
    allowedLabel->setBuddy(m_allowedCharsEdit);
    dictLabel->setBuddy(m_dictionaryEdit);
    
    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(checkerGroup);
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


/*!
    Polishes the dialog, i.e. does some layout settings after the dialog is fully created.
*/
void ConfDlgSecurityTab::polish()
{
    QWidget::polish();
    QFontMetrics fm = fontMetrics();
    int width = fm.boundingRect(m_algorithmLabel->text().replace('&', "")).width();
    m_algorithmLabel->setFixedWidth(width);
    m_lengthSpinner->setFixedWidth(width);
}


/*!
    Applys the settings. This method is called by if the user presses the Ok button.
*/
void ConfDlgSecurityTab::checkboxHandler(bool on)
{
    m_externalEdit->setEnabled(on);
    if (!on)
    {
        m_externalEdit->clearFocus();
    }
}


/*!
    Fills the settings. This method is called from the dialog's constructor.
*/
void ConfDlgSecurityTab::fillSettings()
{
    m_lengthSpinner->setValue(qpamat->set().readNumEntry("Security/Length"));
    m_allowedCharsEdit->setText(qpamat->set().readEntry("Security/AllowedCharacters"));
    m_weakSlider->setValue(int(qpamat->set().readDoubleEntry("Security/WeakPasswordLimit")*2));
    m_strongSlider->setValue(int(qpamat->set().readDoubleEntry("Security/StrongPasswordLimit")*2));
    m_useExternalCB->setChecked(qpamat->set().readEntry("Security/PasswordGenerator") =="EXTERNAL");
    m_externalEdit->setContent(qpamat->set().readEntry("Security/PasswordGenAdditional"));
    m_dictionaryEdit->setContent(qpamat->set().readEntry("Security/DictionaryFile"));
    
    checkboxHandler(m_useExternalCB->isChecked());
    weakSliderHandler(m_weakSlider->value());
    strongSliderHandler(m_strongSlider->value());
    
    m_algorithmCombo->insertStringList(SymmetricEncryptor::getAlgorithms());
    m_algorithmCombo->setCurrentText( qpamat->set().readEntry( "Security/CipherAlgorithm" ));
}

/*!
    Applys the settings. This method is called by if the user presses the Ok button.
*/
void ConfDlgSecurityTab::applySettings()
{
    QString passGen = m_useExternalCB->isChecked() ? "EXTERNAL" : "RANDOM";
    qpamat->set().writeEntry("Security/PasswordGenerator", passGen);
    qpamat->set().writeEntry("Security/PasswordGenAdditional", m_externalEdit->getContent());
    qpamat->set().writeEntry("Security/WeakPasswordLimit", m_weakSlider->value()/2.0);
    qpamat->set().writeEntry("Security/StrongPasswordLimit", m_strongSlider->value()/2.0);
    qpamat->set().writeEntry("Security/Length", m_lengthSpinner->value());
    qpamat->set().writeEntry("Security/AllowedCharacters", m_allowedCharsEdit->text());
    qpamat->set().writeEntry("Security/CipherAlgorithm", m_algorithmCombo->currentText() );
    qpamat->set().writeEntry("Security/DictionaryFile", m_dictionaryEdit->getContent());
}


/*!
    Handles events of the weak slider. If the weak slider is moved, it needs to update the
    strong slider if necessary and to update the LCD on the right.
    \param value the new value
 */
void ConfDlgSecurityTab::weakSliderHandler(int value)
{
    if (m_strongSlider->value() < value)
    {
        m_strongSlider->setValue(value);
    }
    m_weakLabel->display(QString::number(value/2.0, 'f', 1));
}


/*!
    Handles events of the strong slider. If the strong slider is moved, it needs to update the
    weak slider if necessary and to update the LCD on the right.
    \param value the new value
*/
void ConfDlgSecurityTab::strongSliderHandler(int value)
{
    if (m_weakSlider->value() > value)
    {
        m_weakSlider->setValue(value);
    }
    m_strongLabel->display(QString::number(value/2.0, 'f', 1));
}


// -------------------------------------------------------------------------------------------------
//                                     Presentation tab
// -------------------------------------------------------------------------------------------------


/*!
    \class ConfDlgPresentationTab
    
    \brief Represents presentation settings in the configuration dialog
    
    This tab holds all presentation settings 
    
     - displaying of passwords in the RandomPasswordDialog
     - fonts used for printing
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.13 $
    \date $Date: 2003/12/28 22:08:15 $
*/


/*!
    Creates a new instance of an ConfDlgPresentationTab object.
    \param parent the parent widget
*/
ConfDlgPresentationTab::ConfDlgPresentationTab(QWidget* parent)
    : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
}


/*!
    Does creating of subwidgets and layout of the GeneralTab.
*/
void ConfDlgPresentationTab::createAndLayout()
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


/*!
    Fills the settings. This method is called from the dialog's constructor.
*/
void ConfDlgPresentationTab::fillSettings()
{
    QFont font;
    
    font.fromString(qpamat->set().readEntry( "Presentation/NormalFont"));
    m_normalFontEdit->setFont(font);
    font.fromString(qpamat->set().readEntry( "Presentation/FooterFont"));
    m_footerFontEdit->setFont(font);
    m_hidePasswordCB->setChecked(qpamat->set().readBoolEntry("Presentation/HideRandomPass"));
}


/*!
    Applys the settings. This method is called by if the user presses the Ok button.
*/
void ConfDlgPresentationTab::applySettings()
{
    qpamat->set().writeEntry("Presentation/HideRandomPass", m_hidePasswordCB->isChecked());
    qpamat->set().writeEntry("Presentation/NormalFont", m_normalFontEdit->getFont().toString());
    qpamat->set().writeEntry("Presentation/FooterFont", m_footerFontEdit->getFont().toString());
}


// -------------------------------------------------------------------------------------------------
//                                     Smart Card
// -------------------------------------------------------------------------------------------------


/*!
    \class ConfDlgSmartcardTab
    
    \brief Represents smartcard settings in the configuration dialog
    
    This tab holds all smartcard settings 
    
     - library
     - port
     - testing faclity
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.13 $
    \date $Date: 2003/12/28 22:08:15 $
*/

/*!
    \enum ConfDlgSmartcardTab::SmartcardEnabled
    
    Enumeration type to represent enabled/not enabled as radio button group.
*/ 

/*!
    Creates a new instance of an ConfDlgSmartcardTab object.
    \param parent the parent widget
*/
ConfDlgSmartcardTab::ConfDlgSmartcardTab(QWidget* parent)
        : QWidget(parent)
{
    createAndLayout();
    
    // init contents
    fillSettings();
    
    connect(m_radioGroup, SIGNAL(clicked(int)), this, SLOT(radioButtonHandler(int)));
    connect(m_testButton, SIGNAL(clicked()), this, SLOT(testSmartCard()));
}


/*!
    Does creating of subwidgets and layout of the GeneralTab.
*/
void ConfDlgSmartcardTab::createAndLayout()
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


/*!
    Handles the radio button. If the "enabled" button is activated, the configuration widgets get
    displayed, otherwise they get hidden.
    \param buttonId the id of the radio button, in reality of type SmartcardEnabled
*/
void ConfDlgSmartcardTab::radioButtonHandler(int buttonId)
{
    bool enabled = SmartcardEnabled(buttonId) == Enabled;
    m_settingsGroup->setEnabled(enabled);
    m_testGroup->setEnabled(enabled);
    m_radioGroup->setButton( enabled ? Enabled : NotEnabled );
}


/*!
    Fills the settings. This method is called from the dialog's constructor.
*/
void ConfDlgSmartcardTab::fillSettings()
{
    m_portCombo->clear();
    const char* ports[] = { QT_TR_NOOP("Special (Port 0)"), QT_TR_NOOP("COM1/USB/Keyboard"), 
        QT_TR_NOOP("COM2"), QT_TR_NOOP("COM3"), QT_TR_NOOP("COM4") };
    for (int i = 0; i < 5; ++i)
    {
        m_portCombo->insertItem(tr(ports[i]));
    }
    
    m_libraryEdit->setContent(qpamat->set().readEntry("Smartcard/Library"));
    m_portCombo->setCurrentItem(qpamat->set().readNumEntry("Smartcard/Port"));
    radioButtonHandler(qpamat->set().readBoolEntry("Smartcard/UseCard") ? Enabled : NotEnabled);
}


/*!
    Applys the settings. This method is called by if the user presses the Ok button.
*/
void ConfDlgSmartcardTab::applySettings()
{
    qpamat->set().writeEntry("Smartcard/Library", m_libraryEdit->getContent() );
    qpamat->set().writeEntry("Smartcard/Port", m_portCombo->currentItem() );
    qpamat->set().writeEntry("Smartcard/UseCard", 
        SmartcardEnabled(m_radioGroup->selectedId()) == Enabled);
}


/*!
    Tests the smartcard. This is to verify the settings.
    It tries to read from the reader. It checks the type of the smartcard and if it's a memory
    card, it displays the avalable memory in a dialog.
*/
void ConfDlgSmartcardTab::testSmartCard()
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

#endif // DOXYGEN
