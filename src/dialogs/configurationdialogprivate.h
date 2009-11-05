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
#ifndef CONFIGURATIONDIALOGPRIVATE_H
#define CONFIGURATIONDIALOGPRIVATE_H

#include <QLineEdit>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <Q3ButtonGroup>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLCDNumber>

#include "widgets/filelineedit.h"
#include "widgets/fontchoosebox.h"
#include "widgets/listboxdialog.h"


class ConfDlgGeneralTab : public ListBoxDialogPage
{
    Q_OBJECT

    friend class ConfigurationDialog;

    public:
        ConfDlgGeneralTab (QWidget* parent, const char* name);

    protected:
        void fillSettings();
        void applySettings();

    private:
        void createAndLayout();

    private:
        QCheckBox*      m_autoLoginCheckbox;
        FileLineEdit*   m_datafileEdit;
        QLineEdit*      m_miscEdit;
        QLineEdit*      m_usernameEdit;
        QLineEdit*      m_passwordEdit;
        QLineEdit*      m_urlEdit;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgPasswordTab : public ListBoxDialogPage
{
    Q_OBJECT

    friend class ConfigurationDialog;

    public:
        ConfDlgPasswordTab(QWidget* parent, const char* name = 0);

    protected slots:
        void checkboxHandler(bool on);

    protected:
        void fillSettings();
        void applySettings();

    private slots:
        void weakSliderHandler(int value);
        void strongSliderHandler(int value);
        void sortDictionary();

    private:
        void createAndLayout();

    private:
        // passwords
        QSpinBox*       m_lengthSpinner;
        FileLineEdit*   m_externalEdit;
        QLineEdit*      m_allowedCharsEdit;
        QCheckBox*      m_useExternalCB;
        // checker
        QSlider*        m_weakSlider;
        QSlider*        m_strongSlider;
        QLCDNumber*     m_weakLabel;
        QLCDNumber*     m_strongLabel;
        FileLineEdit*   m_dictionaryEdit;
        QPushButton*    m_sortButton;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgSecurityTab : public ListBoxDialogPage
{
    Q_OBJECT

    friend class ConfigurationDialog;

    public:
        ConfDlgSecurityTab(QWidget* parent, const char* name = 0);

    protected:
        void fillSettings();
        void applySettings();

    private:
        void createAndLayout();

    protected:
        static const int m_minuteMap[];

    private:
        // algorithm
        QComboBox*      m_algorithmCombo;
        QLabel*         m_algorithmLabel;
        // logout
        QComboBox*      m_logoutCombo;
        QLabel*         m_logoutLabel;
};


// -------------------------------------------------------------------------------------------------

class ConfDlgSmartcardTab : public ListBoxDialogPage
{
    Q_OBJECT

    friend class ConfigurationDialog;

    public:
        enum SmartcardEnabled { NotEnabled = 0, Enabled = 1 };

    public:
        ConfDlgSmartcardTab(QWidget* parent, const char* name = 0);

    protected slots:
        void testSmartCard();

    protected:
        void fillSettings();
        void applySettings();

    private slots:
        void setUseSmartcardEnabled(bool enabled);

    private:
        void createAndLayout();

    private:
        QCheckBox*      m_useCardCB;
        QCheckBox*      m_usePinCB;
        Q3GroupBox*      m_settingsGroup;
        Q3GroupBox*      m_testGroup;
        FileLineEdit*   m_libraryEdit;
        QComboBox*      m_portCombo;
        QPushButton*    m_testButton;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgPresentationTab : public ListBoxDialogPage
{
    Q_OBJECT

    public:
        ConfDlgPresentationTab(QWidget* parent, const char* name = 0);

    protected:
        void fillSettings();
        void applySettings();

    private:
        void createAndLayout();

    private:
        FontChooseBox*  m_normalFontEdit;
        FontChooseBox*  m_footerFontEdit;
        QCheckBox*      m_hidePasswordCB;
        QCheckBox*      m_nograbCB;
        QCheckBox*      m_systrayCB;
        QCheckBox*      m_hiddenCB;
};


#endif // CONFIGURATIONDIALOGPRIVATE_H

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
