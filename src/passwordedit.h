/*
 * Id: $Id: passwordedit.h,v 1.1 2003/09/14 17:10:31 bwalle Exp $
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
#include <qobject.h>
#include <qlineedit.h>
#include <qstring.h>

/**
 * @brief Enhanced and limited QLineEdit widget for entering passwords.
 */
class PasswordEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QChar fillCharacter READ fillCharacter WRITE setFillCharacter) 

    public:
        PasswordEdit(QWidget*, const char* = NULL);
        PasswordEdit(const QString&, QWidget*, const char* = NULL);
        
        QChar fillCharacter() const;
        void setFillCharacter(const QChar&);

    protected:
        virtual void keyPressEvent (QKeyEvent*);

    private:
        void init();
        
        QString m_password;
        QChar   m_fillCharacter;
        QString m_displayedPassword;
};

