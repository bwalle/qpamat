/*
 * Id: $Id: newpassworddialog.h,v 1.5 2003/12/28 22:08:15 bwalle Exp $
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
        
    private:
        void createAndLayout();
        
    private:
        QString         m_oldPassword;
        QPushButton*    m_okButton;
        QPushButton*    m_cancelButton;
        QLineEdit*      m_firstPasswordEdit;
        QLineEdit*      m_secondPasswordEdit;
        QLineEdit*      m_oldPasswordEdit;
};

#endif // NEWPASSWORDDIALOG_H
