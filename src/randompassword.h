/*
 * Id: $Id: randompassword.h,v 1.2 2003/12/17 21:57:28 bwalle Exp $
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
#ifndef RANDOMPASSWORD_H
#define RANDOMPASSWORD_H

#include <qobject.h>
#include <qstring.h>

class Qpamat;

/**
 * \brief This class handles RandomPasswords in the QPaMaT application.
 *
 * It doesn't generate RandomPasswords but it manages displaying and sending them to other
 * widgets.
 *
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/17 21:57:28 $
 */
class RandomPassword : public QObject
{
    Q_OBJECT
    
    public:
        
        /*!
         * Creates a new instance of a RandomPassword object. Inserting is not enabled initally.
         * \param parent the parent object
         * \param name the name of the object
         */
        RandomPassword(Qpamat* parent, const char* name = 0);
    
    public slots:
        
        /*!
         * Enables the insert function.
         * \param enabled if the insert function should be enabled
         */
        void setInsertEnabled(bool enabled = true);
        
        /*!
         * Call this slot if you want a random password. A password dialog is displayed that
         * lets the user copy the password into the clipboard or insert (if possible) at the
         * current password QLineEdit.
         */
        void requestPassword();
        
    signals:
        
        /*!
         * This signal is emitted every time the user wants to insert the password at current
         * position.
         */
        void insertPassword(const QString& password);
        
        
    private:
        bool m_insertEnabled;
        Qpamat* m_parent;
};

#endif // RANDOMPASSWORD_H
