/*
 * Id: $Id: passworddialog.h,v 1.3 2003/11/29 14:43:03 bwalle Exp $
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
#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>

/*!
 * \brief Dialog which is used to enter the password
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.3 $
 * \date $Date: 2003/11/29 14:43:03 $
 */
class PasswordDialog : public QDialog 
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of the password dialog.
         * \param parent the parent widget
         */
        PasswordDialog(QWidget* parent);
        
        /*!
         * Returns the password which the user entered.
         * \return the password
         */
        QString getPassword() const;
        
    private:
        QLineEdit* m_passwordEdit;
};

#endif // PASSWORDDIALOG_H
