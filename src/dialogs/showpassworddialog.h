/*
 * Id: $Id: showpassworddialog.h,v 1.1 2003/12/21 20:29:07 bwalle Exp $
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
#ifndef SHOWPASSWORDDIALOG_H
#define SHOWPASSWORDDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>

#include "../widgets/copylabel.h"

/*!
 * \brief Dialog which displayes a password.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/21 20:29:07 $
 */
class ShowPasswordDialog : public QDialog 
{
    Q_OBJECT
    
    public:
        
        /*! 
         * Datatype for the type of the dialog. A RandomPasswordDialog shows the another description
         * than a normal password dialog. Normal Password dialogs always show the password as
         * cleartext and have no insert button.
         */
        enum DialogType
        {
            TRandomPasswordDlg,       //!< Dialog for showing random passwords with no insert button
            TRandomPasswordDlgInsert, //!< Dialgo for showing random passwords with insert button
            TNormalPasswordDlg        //!< Dialog for showing other passwords (always cleartext)
        };
        
        /*!
         * Creates a new instance of the password dialog.
         * \param parent the parent widget
         * \param type the type of the dialog
         * \param name the name of the object
         */
        ShowPasswordDialog(QWidget* parent, DialogType type,  const char* name = 0);
        
    public slots:
        
        /*!
         * Returns the password which the user entered.
         * \return the password
         */
        QString getPassword() const;
        
        
        /*!
         * Sets the displayed password.
         * \param newPassword the new password
         */
        void setPassword(const QString& newPassword);
        
    signals:
        
        /*!
         * This signal is emitted if the user wants the next random password, usually by
         * pressing a "Next" button.
         */
        void nextRequested();
        
        /*!
         * This signal is emitted if the user wants to insert the password at the current
         * position.
         * \param password the password that should be inserted.
         */
        void insertPassword(const QString& password);
        
    private slots:
        void insertButtonHandler();
        void copyPassword();
        
    private:
        CopyLabel* m_passwordEdit;
};

#endif // SHOWPASSWORDDIALOG_H
