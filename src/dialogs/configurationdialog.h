/*
 * Id: $Id: configurationdialog.h,v 1.2 2003/11/29 14:43:03 bwalle Exp $
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
#include <qpushbutton.h>

#include "../widgets/filelineedit.h"

namespace ConfigurationDialogLocal
{
    class GeneralTab;
    class SmartcardTab;
}

/*!
 * \brief This dialog lets the user change the configuration.
 * 
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/11/29 14:43:03 $
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
        ConfigurationDialogLocal::SmartcardTab* m_smartCardTab;
        
};

/*!
 * This namespace contains implementation details of the ConfigurationDialog. I didn't want to
 * waste the global namespace with this.
 */
namespace ConfigurationDialogLocal
{
    /*! 
     * \brief This class represents the GeneralTab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.2 $
     * \date $Date: 2003/11/29 14:43:03 $
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
            FileLineEdit* m_browserEdit;
            FileLineEdit* m_datafileEdit;
            QComboBox* m_algorithmCombo;
    };
    
    /*! 
     * This class represents the Smartcard tab of the configuration dialog.
     *
     * \ingroup gui
     * \author Bernhard Walle
     * \version $Revision: 1.2 $
     * \date $Date: 2003/11/29 14:43:03 $
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
}

#endif // CONFIGURATIONDIALOG_H
