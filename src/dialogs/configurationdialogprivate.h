/*
 * Id: $Id: configurationdialogprivate.h,v 1.5 2003/12/31 16:33:33 bwalle Exp $
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
#ifndef CONFIGURATIONDIALOGPRIVATE_H
#define CONFIGURATIONDIALOGPRIVATE_H

#include <qlineedit.h>
#include <qslider.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlcdnumber.h>

#include "widgets/filelineedit.h"
#include "widgets/fontchoosebox.h"


// -------------------------------------------------------------------------------------------------

class ConfDlgTab : public QWidget
{
    friend class ConfigurationDialog;
    
    public:
        ConfDlgTab(QWidget* parent, const char* name = 0) : QWidget(parent, name) { }
        
    protected:
        virtual void fillSettings() = 0;
        virtual void applySettings() = 0;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgGeneralTab : public ConfDlgTab
{
    friend class ConfigurationDialog;
    
    public:
        ConfDlgGeneralTab (QWidget* parent);
        
    protected:
        void fillSettings();
        void applySettings();
        
    private:
        void createAndLayout();
        
    private:
        QCheckBox*      m_autoLoginCheckbox;
        FileLineEdit*   m_browserEdit;
        FileLineEdit*   m_datafileEdit;
        QLineEdit*      m_miscEdit;
        QLineEdit*      m_usernameEdit;
        QLineEdit*      m_passwordEdit;
        QLineEdit*      m_urlEdit;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgPasswordTab : public ConfDlgTab
{
    Q_OBJECT
    
    friend class ConfigurationDialog;
    
    public:
        ConfDlgPasswordTab(QWidget* parent);
        
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

class ConfDlgSecurityTab : public ConfDlgTab
{
    friend class ConfigurationDialog;
    
    public:
        ConfDlgSecurityTab(QWidget* parent);
    
    protected:
        void fillSettings();
        void applySettings();
        
    private:
        void createAndLayout();
        
    private:
        // algorithm
        QComboBox*      m_algorithmCombo;
        QLabel*         m_algorithmLabel;
};


// -------------------------------------------------------------------------------------------------

class ConfDlgSmartcardTab : public ConfDlgTab
{
    Q_OBJECT
    
    friend class ConfigurationDialog;
    
    public:
        enum SmartcardEnabled { NotEnabled = 0, Enabled = 1 };
    
    public:
        ConfDlgSmartcardTab(QWidget* parent);
    
    protected slots:
        void testSmartCard();
        
    protected:
        void fillSettings();
        void applySettings();
        
    private slots:
        void radioButtonHandler(int buttonId);
        
    private:
        void createAndLayout();
        void setUseSmartcardEnabled(bool enabled);
        
    private:
        QButtonGroup*   m_radioGroup;
        QGroupBox*      m_settingsGroup;
        QGroupBox*      m_testGroup;
        FileLineEdit*   m_libraryEdit;
        QComboBox*      m_portCombo;
        QPushButton*    m_testButton;
};

// -------------------------------------------------------------------------------------------------

class ConfDlgPresentationTab : public ConfDlgTab
{
    public:
        ConfDlgPresentationTab(QWidget* parent);
    
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
};


#endif // CONFIGURATIONDIALOGPRIVATE_H
