/*
 * Id: $Id: randompassworddialog.h,v 1.1 2003/12/16 22:49:55 bwalle Exp $
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
#ifndef RANDOMPASSWORDDIALOG_H
#define RANDOMPASSWORDDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>

#include "../widgets/copylineedit.h"

/*!
 * \brief Dialog which displayes a random password.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/16 22:49:55 $
 */
class RandomPasswordDialog : public QDialog 
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of the password dialog.
         * \param parent the parent widget
         * \param showInsertButton whether there should be displayed a insert button. If so, it 
         *        makes sense that the user connects to the insertPassword() slot.
         * \param name the name of the object
         */
        RandomPasswordDialog(QWidget* parent, bool showInsertButton, const char* name = 0);
        
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
        
    private:
        CopyLineEdit* m_passwordEdit;
};

#endif // RANDOMPASSWORDDIALOG_H
