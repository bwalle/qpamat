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
#include <algorithm>

#include <QWidget>
#include <Q3TabDialog>
#include <QLineEdit>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <Q3GroupBox>
#include <Q3WhatsThis>
#include <QLayout>
#include <QRadioButton>
#include <Q3ButtonGroup>
#include <Q3HBox>
#include <Q3VBox>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLCDNumber>
#include <Q3WidgetStack>
#include <Q3ListBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "global.h"
#include "configurationdialog.h"
#include "configurationdialogprivate.h"
#include "qpamatwindow.h"
#include "qpamat.h"
#include "widgets/filelineedit.h"
#include "widgets/listboxlabeledpict.h"
#include "security/passwordgeneratorfactory.h"
#include "security/symmetricencryptor.h"
#include "security/hybridpasswordchecker.h"
#include "smartcard/memorycard.h"

/**
 * @class ConfigurationDialog
 *
 * @brief This dialog displays the configuration and lets the user change it.
 *
 * The whole configuration is done by this dialog. There's not very much to
 * say, the dialog has four tabs:
 *
 *  - @b General which describes general display options
 *  - @b Password which does the password settings
 *  - @b Security which does the security settings
 *  - @b SmartCard which controls all stuff about smartcards
 *  - @b Presentation which holds all about printing and some other things
 *
 * Just use following to use the dialog:
 *
 * @code
 * #include <QScopedPointer>
 *
 * QScopedPointer<ConfigurationDialog> dlg(new ConfigurationDialog(this));
 * if (dlg->exec() == QDialog::Accepted) {
 *     // do something
 * }
 * @endcode
 *
 * Using QScopedPointer or std::auto_ptr here is a good idea but not neccessary.
 *
 * @ingroup dialogs
 * @author Bernhard Walle
 */

/**
 * Creates a new instance of a ConfigurationDialog.
 */
ConfigurationDialog::ConfigurationDialog(QWidget* parent)
    : ListBoxDialog(parent)
{
    setCaption("QPaMaT");

    // Add the general tab
    ConfDlgGeneralTab* generalTab = new ConfDlgGeneralTab(this, "GeneralTab");
    addPage(generalTab, QPixmap(":/images/general_34.png"), tr("General"));

    // Add the password tab
    ConfDlgPasswordTab* passwordTab = new ConfDlgPasswordTab(this, "PasswordTab");
    addPage(passwordTab, QPixmap(":/images/password_34.png"), tr("Password"));

    // Add the security tab
    ConfDlgSecurityTab* securityTab = new ConfDlgSecurityTab(this, "SecurityTab");
    addPage(securityTab, QPixmap(":/images/qpamat_34.png"), tr("Security"));

    // Add the smartcard tab
    ConfDlgSmartcardTab* smartCardTab = new ConfDlgSmartcardTab(this, "SmartCardTab");
    addPage(smartCardTab, QPixmap(":/images/smartcard_34.png"), tr("Smart Card"));

    // Add the presentation tab
    ConfDlgPresentationTab* presentationTab = new ConfDlgPresentationTab(this, "PresTab");
    addPage(presentationTab, QPixmap(":/images/presentation_34.png"), tr("Presentation"));

    adjustSize();
}

#ifndef DOXYGEN

// -------------------------------------------------------------------------------------------------
//                                     Configuration dialog tab
// -------------------------------------------------------------------------------------------------

/**
 *
 * @class ConfDlgTab
 *
 * @brief Base class for all tabs displayed in the configuration dialog
 *
 * This is a interface that all tabs in the configuration dialog must implement.
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @fn ConfDlgTab::ConfDlgTab(QWidget*, const char*)
 *
 * Creates a new instance of a ConfDlgTab
 *
 * @param parent the parent widget
 * @param name the name of the widget
 */

/**
 * @fn ConfDlgTab::fillSettings()
 *
 * This function reads all settings from the global Settings object and fills the GUI with
 * this values.
 */

/**
 * @fn ConfDlgTab::applySettings()
 *
 * This function stores all settings made in the GUI to the global Settings object.
 */

// -------------------------------------------------------------------------------------------------
//                                     General tab
// -------------------------------------------------------------------------------------------------

/**
 * @class ConfDlgGeneralTab
 *
 * @brief Represents general settings in the configuration dialog
 *
 * This tab holds all general settings
 *
 *   - startup (AutoLogin)
 *   - locations of external applications
 *   - AutoText function
 *
 * @ingroup gui
 * @author Bernhard Walle
 */


/**
 * @brief Creates a new instance of an ConfDlgGeneralTab object.
 *
 * @param parent the parent widget
 */
ConfDlgGeneralTab::ConfDlgGeneralTab(QWidget* parent, const char* name)
    : ListBoxDialogPage(parent, name)
{
    createAndLayout();
}


/**
 * @brief Does creating of subwidgets and layout of the GeneralTab.
 */
void ConfDlgGeneralTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 6);
    Q3GroupBox* startupGroup = new Q3GroupBox(1, Qt::Vertical, tr("Startup"), this);
    Q3GroupBox* locationsGroup = new Q3GroupBox(4, Qt::Vertical, tr("Locations"), this);
    Q3GroupBox* autoTextGroup = new Q3GroupBox(4, Qt::Vertical, tr("AutoText"), this);

    // auto login
    m_autoLoginCheckbox = new QCheckBox(tr("Enable &AutoLogin on startup"), startupGroup);

    QLabel* datafileLabel = new QLabel(tr("&Data File:"), locationsGroup);
    m_datafileEdit = new FileLineEdit(locationsGroup, true);

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


/**
 * @brief Fills the settings.
 *
 * This method is called from the dialog's constructor.
 */
void ConfDlgGeneralTab::fillSettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();

    m_autoLoginCheckbox->setChecked(win->set().readBoolEntry("General/AutoLogin"));
    m_datafileEdit->setContent(win->set().readEntry("General/Datafile"));
    m_miscEdit->setText(win->set().readEntry("AutoText/Misc"));
    m_usernameEdit->setText(win->set().readEntry("AutoText/Username"));
    m_passwordEdit->setText(win->set().readEntry("AutoText/Password"));
    m_urlEdit->setText(win->set().readEntry("AutoText/URL"));
}


/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgGeneralTab::applySettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();

    win->set().writeEntry("General/AutoLogin", m_autoLoginCheckbox->isChecked() );
    win->set().writeEntry("General/Datafile", m_datafileEdit->getContent() );
    win->set().writeEntry("AutoText/Misc", m_miscEdit->text() );
    win->set().writeEntry("AutoText/Username", m_usernameEdit->text() );
    win->set().writeEntry("AutoText/Password", m_passwordEdit->text() );
    win->set().writeEntry("AutoText/URL", m_urlEdit->text() );
}


// -------------------------------------------------------------------------------------------------
//                                     Password tab
// -------------------------------------------------------------------------------------------------

/**
 * @class ConfDlgPasswordTab
 *
 * @brief Represents password settings in the configuration dialog
 *
 * This tab holds security general settings
 *
 *   - settings for generated passwords
 *   - password strength settings
 *
 * @ingroup gui
 * @author Bernhard Walle
 */


/**
 * Creates a new instance of an ConfDlgPasswordTab object.
 *
 * @param parent the parent widget
 */
ConfDlgPasswordTab::ConfDlgPasswordTab(QWidget* parent, const char* name)
    : ListBoxDialogPage(parent, name)
    , m_lengthSpinner(0)
    , m_externalEdit(0)
    , m_allowedCharsEdit(0)
    , m_useExternalCB(0)
    , m_weakSlider(0)
    , m_strongSlider(0)
    , m_weakLabel(0)
    , m_strongLabel(0)
    , m_sortButton(0)
{
    createAndLayout();

    connect(m_useExternalCB, SIGNAL(toggled(bool)), SLOT(checkboxHandler(bool)));
    connect(m_weakSlider, SIGNAL(valueChanged(int)), SLOT(weakSliderHandler(int)));
    connect(m_strongSlider, SIGNAL(valueChanged(int)), SLOT(strongSliderHandler(int)));
    connect(m_sortButton, SIGNAL(clicked()), SLOT(sortDictionary()));
}


/**
 * @brief Does creating of subwidgets and layout of the GeneralTab.
 */
void ConfDlgPasswordTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 6);
    Q3GroupBox* passwordGroup = new Q3GroupBox(5, Qt::Vertical, tr("Generated Passwords"), this);
    Q3GroupBox* checkerGroup = new Q3GroupBox(6, Qt::Vertical, tr("Password checker"), this);

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
    Q3HBox* weakSliderBox = new Q3HBox(checkerGroup, "WeakSliderBox");
    weakSliderBox->setSpacing(4);
    m_weakSlider = new QSlider(0, 100, 1, 2, Qt::Horizontal, weakSliderBox, "WeakSlider");
    m_weakLabel = new QLCDNumber(3, weakSliderBox);
    m_weakLabel->setSmallDecimalPoint(true);
    m_weakLabel->setLineWidth(0);

    Q3HBox* strongSliderBox = new Q3HBox(checkerGroup, "WeakSliderBox");
    strongSliderBox->setSpacing(4);
    m_strongSlider = new QSlider(0, 100, 1, 15, Qt::Horizontal, strongSliderBox, "WeakSlider");
    m_strongLabel = new QLCDNumber(3, strongSliderBox);
    m_strongLabel->setSmallDecimalPoint(true);
    m_strongLabel->setLineWidth(0);

    QLabel* dictLabel = new QLabel(tr("&Dictionary file (must be sorted once):"),
            checkerGroup, "DictLabel");
    m_dictionaryEdit = new FileLineEdit(checkerGroup, false, "DictEdit");

    // sort button
    Q3HBox* box = new Q3HBox(checkerGroup, "Hbox");
    m_sortButton = new QPushButton(tr("&Sort dictionary"), box, "SortBtn");
    m_sortButton->setAutoDefault(false);

    QWidget* dummy = new QWidget(box);
    box->setStretchFactor(dummy, 10);

    // buddys
    lengthLabel->setBuddy(m_lengthSpinner);
    allowedLabel->setBuddy(m_allowedCharsEdit);
    dictLabel->setBuddy(m_dictionaryEdit);

    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(checkerGroup);
    mainLayout->addStretch(5);

    // help
    Q3WhatsThis::add(m_sortButton, tr("<qt>For performance reasons, the dictionary file needs "
        "to be sorted by the length of the words. This function does that!<p>It saves also a "
        "copy of the old file by <i>filename.old</i>.</qt>"));
}


/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgPasswordTab::checkboxHandler(bool on)
{
    m_externalEdit->setEnabled(on);
    if (!on)
        m_externalEdit->clearFocus();
}


/**
 * @brief Fills the settings.
 *
 * This method is called from the dialog's constructor.
 */
void ConfDlgPasswordTab::fillSettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();

    m_lengthSpinner->setValue(win->set().readNumEntry("Security/Length"));
    m_allowedCharsEdit->setText(win->set().readEntry("Security/AllowedCharacters"));
    m_weakSlider->setValue(int(win->set().readDoubleEntry("Security/WeakPasswordLimit")*2));
    m_strongSlider->setValue(int(win->set().readDoubleEntry("Security/StrongPasswordLimit")*2));
    m_useExternalCB->setChecked(win->set().readEntry("Security/PasswordGenerator") =="EXTERNAL");
    m_externalEdit->setContent(win->set().readEntry("Security/PasswordGenAdditional"));
    m_dictionaryEdit->setContent(win->set().readEntry("Security/DictionaryFile"));

    checkboxHandler(m_useExternalCB->isChecked());
    weakSliderHandler(m_weakSlider->value());
    strongSliderHandler(m_strongSlider->value());
}

/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgPasswordTab::applySettings()
{
    QString passGen = m_useExternalCB->isChecked() ? "EXTERNAL" : "RANDOM";
    QpamatWindow *win = Qpamat::instance()->getWindow();

    win->set().writeEntry("Security/PasswordGenerator", passGen);
    win->set().writeEntry("Security/PasswordGenAdditional", m_externalEdit->getContent());
    win->set().writeEntry("Security/WeakPasswordLimit", m_weakSlider->value()/2.0);
    win->set().writeEntry("Security/StrongPasswordLimit", m_strongSlider->value()/2.0);
    win->set().writeEntry("Security/Length", m_lengthSpinner->value());
    win->set().writeEntry("Security/AllowedCharacters", m_allowedCharsEdit->text());
    win->set().writeEntry("Security/DictionaryFile", m_dictionaryEdit->getContent());
}


/**
 * @brief Handles events of the weak slider.
 *
 * If the weak slider is moved, it needs to update the strong slider if
 * necessary and to update the LCD on the right.
 *
 * @param value the new value
 */
void ConfDlgPasswordTab::weakSliderHandler(int value)
{
    if (m_strongSlider->value() < value)
        m_strongSlider->setValue(value);
    m_weakLabel->display(QString::number(value/2.0, 'f', 1));
}


/**
 * @brief Handles events of the strong slider.
 *
 * If the strong slider is moved, it needs to update the weak slider if
 * necessary and to update the LCD on the right.
 *
 * @param value the new value
 */
void ConfDlgPasswordTab::strongSliderHandler(int value)
{
    if (m_weakSlider->value() > value)
        m_weakSlider->setValue(value);
    m_strongLabel->display(QString::number(value/2.0, 'f', 1));
}


/**
 * @brief Sorts the dictionary that was specified in the dictionary line edit.
 *
 * Stores a backup copy of the file in filename.bak.
 */
void ConfDlgPasswordTab::sortDictionary()
{
    StringVector words;
    QFile file(m_dictionaryEdit->getContent());
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "QPaMaT", tr("The file you wanted to sort does not exist!"),
            QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    QTextStream stream(&file);
    while (!stream.atEnd())
        words.append(stream.readLine());
    file.close();

    std::sort(words.begin(), words.end(), string_length_greater);

    // save the old file
    if (!QDir::root().rename(file.name(), file.name() + ".bak")) {
        QApplication::restoreOverrideCursor();
        if (QMessageBox::question(this, "QPaMaT", tr("Failed to create a backup file. Do you want\n"
                "to continue without a backup?"), QMessageBox::Yes,
                QMessageBox::No | QMessageBox::Default) != QMessageBox::Yes)
            return;
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, "QPaMaT", tr("Could not open the file for writing!"),
            QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QTextStream writestream(&file);

    for (StringVector::iterator it = words.begin(); it != words.end(); ++it)
        writestream << *it << endl;

    QApplication::restoreOverrideCursor();
}


// -------------------------------------------------------------------------------------------------
//                                     Security tab
// -------------------------------------------------------------------------------------------------

/**
 * @class ConfDlgPasswordTab
 *
 * @brief Represents security settings in the configuration dialog
 *
 * This tab holds security general settings
 *
 *   - cipher algorithm
 *   - automatic logout
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @brief Maps combo box index to minutes.
 */
const int ConfDlgSecurityTab::m_minuteMap[] = {
    0,          /**< disabled */
    15,         /**< 15 minutes */
    30,         /**< 30 minutes */
    60,         /**< 60 minutes */
    120         /**< 2 hours */
};

/**
 * @brief Creates a new instance of an ConfDlgPasswordTab object.
 *
 * @param parent the parent widget
 * @param name the name of the widget
 */
ConfDlgSecurityTab::ConfDlgSecurityTab(QWidget* parent, const char* name)
    : ListBoxDialogPage(parent, name)
    , m_algorithmCombo(0)
{
    createAndLayout();
}


/**
 * @brief Does creating of subwidgets and layout of the GeneralTab.
 */
void ConfDlgSecurityTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 6);
    Q3GroupBox* encryptionGroup = new Q3GroupBox(1, Qt::Vertical, tr("Encryption"), this);
    Q3GroupBox* logoutGroup = new Q3GroupBox(1, Qt::Vertical, tr("Logout"), this);

    // algorithm stuff
    m_algorithmLabel = new QLabel(tr("Cipher &algorithm:"), encryptionGroup);
    m_algorithmCombo = new QComboBox(false, encryptionGroup);

    // logout
    m_logoutLabel = new QLabel(tr("Auto &logout after inactivity:"), logoutGroup);
    m_logoutCombo = new QComboBox(false, logoutGroup);

    // buddys
    m_algorithmLabel->setBuddy(m_algorithmCombo);
    m_logoutLabel->setBuddy(m_logoutCombo);

    mainLayout->addWidget(encryptionGroup);
    mainLayout->addWidget(logoutGroup);
    mainLayout->addStretch(5);
}


/**
 * @brief Fills the settings.
 *
 * This method is called from the dialog's constructor.
 */
void ConfDlgSecurityTab::fillSettings()
{
    qDebug() << CURRENT_FUNCTION << "Insert algorithm";
    QpamatWindow *win = Qpamat::instance()->getWindow();

    m_algorithmCombo->insertStringList(SymmetricEncryptor::getAlgorithms());
    m_algorithmCombo->setCurrentText( win->set().readEntry( "Security/CipherAlgorithm" ));

    // Combo box
    m_logoutCombo->insertItem(tr("Disabled"));
    m_logoutCombo->insertItem(tr("15 minutes"));
    m_logoutCombo->insertItem(tr("30 minutes"));
    m_logoutCombo->insertItem(tr("1 hour"));
    m_logoutCombo->insertItem(tr("2 hours"));

    int logout = win->set().readNumEntry( "Security/AutoLogout" );
    int size = sizeof(ConfDlgSecurityTab::m_minuteMap)/sizeof(int);
    const int* val = qFind(
        ConfDlgSecurityTab::m_minuteMap,
        ConfDlgSecurityTab::m_minuteMap + size,
        logout
    );
    m_logoutCombo->setCurrentItem(val - ConfDlgSecurityTab::m_minuteMap);
}

/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgSecurityTab::applySettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();
    int min = ConfDlgSecurityTab::m_minuteMap[m_algorithmCombo->currentItem()];
    win->set().writeEntry("Security/CipherAlgorithm", m_algorithmCombo->currentText() );
    win->set().writeEntry("Security/AutoLogout", min);
}


// -------------------------------------------------------------------------------------------------
//                                     Presentation tab
// -------------------------------------------------------------------------------------------------


/**
 * @class ConfDlgPresentationTab
 *
 * @brief Represents presentation settings in the configuration dialog
 *
 * This tab holds all presentation settings
 *
 *   - displaying of passwords in the RandomPasswordDialog
 *   - fonts used for printing
 *
 * @ingroup gui
 * @author Bernhard Walle
 */


/**
 * @brief Creates a new instance of an ConfDlgPresentationTab object.
 *
 * @param parent the parent widget
 */
ConfDlgPresentationTab::ConfDlgPresentationTab(QWidget* parent, const char* name)
    : ListBoxDialogPage(parent, name)
    , m_nograbCB(0)
{
    createAndLayout();

    connect(m_systrayCB, SIGNAL(toggled(bool)), m_hiddenCB, SLOT(setEnabled(bool)));
}


/**
 * @brief Does creating of subwidgets and layout of the GeneralTab.
 */
void ConfDlgPresentationTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 6);
    Q3GroupBox* passwordGroup = new Q3GroupBox(2, Qt::Vertical, tr("Passwords"), this);
    Q3GroupBox* fontGroup = new Q3GroupBox(4, Qt::Vertical, tr("Printing Fonts"), this);
    Q3GroupBox* systemTrayGroup = new Q3GroupBox(2, Qt::Vertical, tr("System tray"), this);

    QLabel* normalLabel = new QLabel(tr("&Normal font:"), fontGroup);
    m_normalFontEdit = new FontChooseBox(fontGroup);
    QLabel* footerLabel = new QLabel(tr("&Footer font:"), fontGroup);
    m_footerFontEdit = new FontChooseBox(fontGroup);

    m_hidePasswordCB = new QCheckBox(tr("Hide passwords in Random Password Dialog"),
        passwordGroup, "HidePasswords");
    m_nograbCB = new QCheckBox(tr("Don't &grab keyboard focus in the password dialog"),
        passwordGroup, "Nograb");

    // Systray
    m_systrayCB = new QCheckBox(tr("Show icon in system tray (needs restart!)"),
        systemTrayGroup, "IconInSystemtray");
    if (!QSystemTrayIcon::isSystemTrayAvailable())
        m_systrayCB->setEnabled(false);
    m_hiddenCB = new QCheckBox(tr("Start hidden"), systemTrayGroup, "StartHidden");

    // buddys
    normalLabel->setBuddy(m_normalFontEdit);
    footerLabel->setBuddy(m_footerFontEdit);

    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(fontGroup);
    mainLayout->addWidget(systemTrayGroup);
    mainLayout->addStretch(5);

}


/**
 * @brief Fills the settings.
 *
 * This method is called from the dialog's constructor.
 */
void ConfDlgPresentationTab::fillSettings()
{
    QFont font;
    QpamatWindow *win = Qpamat::instance()->getWindow();
    font.fromString(win->set().readEntry( "Presentation/NormalFont"));
    m_normalFontEdit->setFont(font);
    font.fromString(win->set().readEntry( "Presentation/FooterFont"));
    m_footerFontEdit->setFont(font);
    m_hidePasswordCB->setChecked(win->set().readBoolEntry("Presentation/HideRandomPass"));
    m_nograbCB->setChecked(win->set().readBoolEntry("Password/NoGrabbing"));
    m_systrayCB->setChecked(win->set().readBoolEntry("Presentation/SystemTrayIcon"));
    m_hiddenCB->setChecked(win->set().readBoolEntry("Presentation/StartHidden"));
    m_hiddenCB->setEnabled(m_systrayCB->isChecked());
}


/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgPresentationTab::applySettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();

    win->set().writeEntry("Presentation/HideRandomPass", m_hidePasswordCB->isChecked());
    win->set().writeEntry("Password/NoGrabbing", m_nograbCB->isChecked());
    win->set().writeEntry("Presentation/NormalFont", m_normalFontEdit->getFont().toString());
    win->set().writeEntry("Presentation/FooterFont", m_footerFontEdit->getFont().toString());
    win->set().writeEntry("Presentation/SystemTrayIcon", m_systrayCB->isChecked());
    win->set().writeEntry("Presentation/StartHidden", m_hiddenCB->isChecked());
}


// -------------------------------------------------------------------------------------------------
//                                     Smart Card
// -------------------------------------------------------------------------------------------------


/**
 * @class ConfDlgSmartcardTab
 *
 * @brief Represents smartcard settings in the configuration dialog
 *
 * This tab holds all smartcard settings
 *
 *   - library
 *   - port
 *   - testing faclity
 *
 * @ingroup gui
 * @author Bernhard Walle
 */

/**
 * @enum ConfDlgSmartcardTab::SmartcardEnabled
 *
 * Enumeration type to represent enabled/not enabled as radio button group.
 */

/**
 * Creates a new instance of an ConfDlgSmartcardTab object.
 *
 * @param parent the parent widget
 */
ConfDlgSmartcardTab::ConfDlgSmartcardTab(QWidget* parent, const char* name)
        : ListBoxDialogPage(parent, name)
{
    createAndLayout();

    connect(m_useCardCB, SIGNAL(toggled(bool)), this, SLOT(setUseSmartcardEnabled(bool)));
    connect(m_testButton, SIGNAL(clicked()), this, SLOT(testSmartCard()));
}


/**
 * @brief Does creating of subwidgets and layout of the GeneralTab.
 */
void ConfDlgSmartcardTab::createAndLayout()
{
    // create layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 6);

    Q3GroupBox* smartCardGroup = new Q3GroupBox(2, Qt::Vertical, tr("Smartcard"), this);
    m_settingsGroup = new Q3GroupBox(4, Qt::Vertical, tr("Settings"), this);
    m_testGroup = new Q3ButtonGroup(2, Qt::Vertical, tr("Testing"), this);

    m_useCardCB = new QCheckBox(tr("&Use a smartcard"), smartCardGroup);
    m_usePinCB = new QCheckBox(tr("Card has &write-protection"), smartCardGroup);;

    QLabel* libraryLabel = new QLabel(tr("CT-&API Chipcard Driver:"), m_settingsGroup);
    m_libraryEdit = new FileLineEdit(m_settingsGroup, false);

    QLabel* portLabel = new QLabel(tr("Chipcard Terminal &Port:"), m_settingsGroup);
    m_portCombo = new QComboBox(false, m_settingsGroup);

    new QLabel(tr("Insert a card and click on this button to test your settings:"), m_testGroup);
    Q3HBox* box = new Q3HBox(m_testGroup);
    m_testButton = new QPushButton(tr("&Test"), box);
    m_testButton->setAutoDefault(false);

    QWidget* dummy = new QWidget(box);
    box->setStretchFactor(dummy, 10);

    mainLayout->addWidget(smartCardGroup);
    mainLayout->addWidget(m_settingsGroup);
    mainLayout->addWidget(m_testGroup);
    mainLayout->addStretch(5);


    // the case discrimination if rom Qt's code, so it should be correct
#if defined Q_WS_WIN
    m_libraryEdit->setFilter(tr("CT-API-Driver (*.dll)"));
#elif defined(Q_OS_MACX)
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


/**
 * @brief Handles the smart card enabled button.
 *
 * If it is checked, the configuration widgets get displayed, otherwise they
 * get hidden.
 *
 * @param enabled \c true if smart card support is enabled
 */
void ConfDlgSmartcardTab::setUseSmartcardEnabled(bool enabled)
{
    m_usePinCB->setEnabled(enabled);
    m_settingsGroup->setEnabled(enabled);
    m_testGroup->setEnabled(enabled);
}


/**
 * @brief Fills the settings.
 *
 * This method is called from the dialog's constructor.
 */
void ConfDlgSmartcardTab::fillSettings()
{
    m_portCombo->clear();
    const char *ports[] = {
        QT_TR_NOOP("Special (Port 0)"),
        QT_TR_NOOP("COM1/USB/Keyboard"),
        QT_TR_NOOP("COM2"),
        QT_TR_NOOP("COM3"),
        QT_TR_NOOP("COM4")
    };

    for (int i = 0; i < 5; ++i) {
        m_portCombo->insertItem(tr(ports[i]));
    }

    QpamatWindow *win = Qpamat::instance()->getWindow();
    m_libraryEdit->setContent(win->set().readEntry("Smartcard/Library"));
    m_portCombo->setCurrentItem(win->set().readNumEntry("Smartcard/Port"));
    m_useCardCB->setChecked(win->set().readBoolEntry("Smartcard/UseCard"));
    m_usePinCB->setChecked(win->set().readBoolEntry("Smartcard/HasWriteProtection"));

    if (!m_useCardCB->isChecked())
        setUseSmartcardEnabled(false);
}


/**
 * @brief Applys the settings.
 *
 * This method is called by if the user presses the Ok button.
 */
void ConfDlgSmartcardTab::applySettings()
{
    QpamatWindow *win = Qpamat::instance()->getWindow();

    win->set().writeEntry("Smartcard/Library", m_libraryEdit->getContent() );
    win->set().writeEntry("Smartcard/Port", m_portCombo->currentItem() );
    win->set().writeEntry("Smartcard/UseCard", m_useCardCB->isChecked() );
    win->set().writeEntry("Smartcard/HasWriteProtection", m_usePinCB->isChecked() );
}


/**
 * @brief Tests the smartcard.
 *
 * This is to verify the settings.  It tries to read from the reader. It checks
 * the type of the smartcard and if it's a memory card, it displays the
 * avalable memory in a dialog.
 */
void ConfDlgSmartcardTab::testSmartCard()
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    try {
        MemoryCard card(m_libraryEdit->getContent());
        card.init(m_portCombo->currentItem());
        MemoryCard::CardType type = card.getType();

        if (type == MemoryCard::TMemoryCard) {
            int capacity = 0;
            card.resetCard(&capacity);

            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "QPaMaT", QString("<qt>"+tr(
                "<p>Detected a card with %1 kBytes memory.</p><p>If you think this is "
                "enough memory you can use this card for storing your passwords!</p>")+"</qt>").arg(
                QString::number(capacity / 1024.0)),
                QMessageBox::Ok | QMessageBox::Default, Qt::NoButton);
        } else {
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(this, "QPaMaT", QString("<qt>"+tr(
                "<p>The communication to your chipcard terminal seems to work.<p>"
                "<p>However, there's no memory card in your reader. So you cannot use "
                "it for saving your password. Buy a memory card and try again!</p>")),
                QMessageBox::Ok | QMessageBox::Default, Qt::NoButton);
        }

        // if we don't access this point the destructor closes it. So no problem here!
        card.close();
    } catch (const NoSuchLibraryException& ex) {
        QApplication::restoreOverrideCursor();
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", QString("<qt>"+tr(
                "<p>A problem occurred while loading the specified CT-API (chipcard) driver."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    } catch (const CardException& ex) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "QPaMaT", QString("<qt>"+tr(
                "<p>An error occurred while communicating with the chipcard terminal."
                " The error message was:</p><p>%1</p>")+"</qt>").arg(ex.what()),
               QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
    }
}

#endif // DOXYGEN

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
