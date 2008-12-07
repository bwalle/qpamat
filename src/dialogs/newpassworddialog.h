/*
 * Id: $Id$
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

#include <QDialog>
#include <QWidget>

#include "widgets/focuslineedit.h"

class NewPasswordDialog : public QDialog 
{
    Q_OBJECT
    
    public:
        NewPasswordDialog(QWidget* parent, const QString& oldPassword = QString::null);
        QString getPassword() const;
        
    protected slots:
        void accept();
        
    private slots:
        void checkOkEnabled() const;
        void grabOldPassword();
        void grabFirstPassword();
        void grabSecondPassword();
        void release();
        
    private:
        void createAndLayout();
        
    private:
        QString         m_oldPassword;
        QPushButton*    m_okButton;
        QPushButton*    m_cancelButton;
        FocusLineEdit*  m_firstPasswordEdit;
        FocusLineEdit*  m_secondPasswordEdit;
        FocusLineEdit*  m_oldPasswordEdit;
};

#endif // NEWPASSWORDDIALOG_H

// :maxLineLen=100:
