/*
 * Id: $Id: configurationdialog.h,v 1.9 2003/12/18 22:00:02 bwalle Exp $
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
#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <qtabdialog.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>

#include "../widgets/filelineedit.h"
#include "../widgets/fontchoosebox.h"

namespace ConfigurationDialogLocal
{
    class GeneralTab;
    class SecurityTab;
    class SmartcardTab;
    class PresentationTab;
}

/*!
 * \brief This dialog lets the user change the configuration.
 * 
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.9 $
 * \date $Date: 2003/12/18 22:00:02 $
 */
class ConfigurationDialog : public QTabDialog
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of the ConfigurationDialog.
         * \param parent the parent widget
         */
        ConfigurationDialog(QWidget* parent);
        
    private:
        ConfigurationDialogLocal::GeneralTab* m_generalTab;
        ConfigurationDialogLocal::SecurityTab* m_securityTab;
        ConfigurationDialogLocal::SmartcardTab* m_smartCardTab;
        ConfigurationDialogLocal::PresentationTab* m_printingTab;
};

/*!
 * \brief This namespace contains implementation details of the ConfigurationDialog. 
 *
 * I didn't want to waste the global namespace with this.
 */
namespace ConfigurationDialogLocal
{
    /*! 
     * \brief This class represents the GeneralTab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.9 $
     * \date $Date: 2003/12/18 22:00:02 $
     */
    class GeneralTab : public QWidget
    {
        Q_OBJECT
        
        public:
            
            /*!
             * Creates a new instance of the widget.
             * \param parent the parent widget
             */
            GeneralTab (QWidget* parent);
            
        public slots:
            
            /*!
             * Stores the settings.
             */
            void applySettings();
            
        private:
            void createAndLayout();
            void fillSettings();
            
        private:
            QCheckBox* m_autoLoginCheckbox;
            FileLineEdit* m_browserEdit;
            FileLineEdit* m_datafileEdit;
            QLineEdit* m_miscEdit;
            QLineEdit* m_usernameEdit;
            QLineEdit* m_passwordEdit;
            QLineEdit* m_urlEdit;
    };
    
    
    /*! 
     * \brief This class represents the Smartcard tab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.9 $
     * \date $Date: 2003/12/18 22:00:02 $
     */
    class SecurityTab : public QWidget
    {
        Q_OBJECT
        
        public:
            
            /*!
             * Creates a new instance of the widget.
             * \param parent the parent widget
             */
            SecurityTab(QWidget* parent);
        
        public slots:
            
            /*!
             * Stores the settings.
             */
            void applySettings();
            
            /*!
             * Handles changes of the external checkbox.
             * \param on if the checkbox was enabled
             */
            void checkboxHandler(bool on);
            
        private:
            void createAndLayout();
            void fillSettings();
            
        private:
            // passwords
            QSpinBox* m_lengthSpinner;
            FileLineEdit* m_externalEdit;
            QLineEdit* m_allowedCharsEdit;
            QCheckBox* m_useExternalCB;
            QCheckBox* m_uppercaseCB;
            QCheckBox* m_lowercaseCB;
            QCheckBox* m_digitsCB;
            QCheckBox* m_specialCB;
            // algorithm
            QComboBox* m_algorithmCombo;
    };
    
    /*! 
     * \brief This class represents the Smartcard tab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.9 $
     * \date $Date: 2003/12/18 22:00:02 $
     */
    class SmartcardTab : public QWidget
    {
        Q_OBJECT
        
        /*! SmartcardEnabled type. */
        enum SmartcardEnabled { NotEnabled = 0, Enabled = 1 };
        
        public:
            
            /*!
             * Creates a new instance of the widget.
             * \param parent the parent widget
             */
            SmartcardTab(QWidget* parent);
        
        public slots:
            
            /*!
             * Stores the settings.
             */
            void applySettings();
            
            /*!
             * Handles changes of the radio button.
             * \param buttonId the buttonId as returned by the event. In reality this is of type
             *        SmartcardEnabled.
             */
            void radioButtonHandler(int buttonId);
            
            /*!
             * Performs a test if the smart card settings are Ok.
             */
            void testSmartCard();
            
        private:
            void createAndLayout();
            void fillSettings();
            void setUseSmartcardEnabled(bool enabled);
            
        private:
            QButtonGroup* m_radioGroup;
            QGroupBox* m_settingsGroup;
            QGroupBox* m_testGroup;
            FileLineEdit* m_libraryEdit;
            QComboBox* m_portCombo;
            QPushButton* m_testButton;
    };
    
    /*! 
     * \brief This class represents the Priting tab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.9 $
     * \date $Date: 2003/12/18 22:00:02 $
     */
    class PresentationTab : public QWidget
    {
        Q_OBJECT
        
        public:
            
            /*!
             * Creates a new instance of the widget.
             * \param parent the parent widget
             */
            PresentationTab(QWidget* parent);
        
        public slots:
            
            /*!
             * Stores the settings.
             */
            void applySettings();
            
        private:
            void createAndLayout();
            void fillSettings();
            
        private:
            FontChooseBox* m_normalFontEdit;
            FontChooseBox* m_footerFontEdit;
            QCheckBox* m_hidePasswordCB;
    };
}

#endif // CONFIGURATIONDIALOG_H
