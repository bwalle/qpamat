/*
 * Id: $Id: newpassworddialog.h,v 1.1 2003/10/12 15:10:39 bwalle Exp $
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
#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>
#include <qvalidator.h>

/**
 * Dialog which is used to create a new file with a password or to change the password.
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/10/12 15:10:39 $
 */
class NewPasswordDialog : public QDialog 
{
    Q_OBJECT
    
    public:
        
        /**
         * Creates a new instance of the password dialog.
         * @param parent the parent widget
         * @param oldPassword the old password if the password should be changed
         */
        NewPasswordDialog(QWidget* parent, const QString& oldPassword = QString::null);
        
        /**
         * Returns the password which the user entered.
         * @return the password
         */
        QString getPassword() const;
        
    protected slots:
    
        /**
         * Overwritten accept() method. The method is called if Ok is pressed.
         */
        void accept();
        
    private slots:
        void checkOkEnabled() const;
    
    private:
        void createAndLayout();
        
    private:
        QString m_oldPassword;
        QPushButton* m_okButton;
        QPushButton* m_cancelButton;
        QLineEdit* m_firstPasswordEdit;
        QLineEdit* m_secondPasswordEdit;
        QLineEdit* m_oldPasswordEdit;
};

// =================================================================================================

/**
 * Simple password validator. The validator checks if the password is longer than six
 * characters.
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/10/12 15:10:39 $
 */
class PasswordValidator : public QValidator
{
    public:
        /**
         * Creates a new validator. 
         * @param parent the parent
         * @param name the object name
         */
        PasswordValidator(QObject* parent = 0, const char* name = 0);
        
        /**
         * Does the validating.
         * @param input the input
         * @param pos the position of the new character
         * @return the result of validation
         */
        State validate(QString& input, int& pos) const;
};
    
#endif // NEWPASSWORDDIALOG_H
