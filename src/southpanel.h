/*
 * Id: $Id: southpanel.h,v 1.7 2003/12/11 22:02:09 bwalle Exp $
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
#ifndef SOUTHPANEL_H
#define SOUTHPANEL_H

#include <qwidget.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qevent.h>
#include <qtoolbutton.h>

#include "property.h"

/*!
 * \brief Represents the south panel.
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.7 $
 * \date $Date: 2003/12/11 22:02:09 $
 */
class SouthPanel : public QFrame
{
    Q_OBJECT
    
    public:
        /*!
         * Creates a new instance of the south panel.
         * \param parent the parent
         */
        SouthPanel(QWidget* parent);
        
        /*!
         * Deletes the South Panel.
         */
        virtual ~SouthPanel() { }
        
        /*!
         * Returns if the focus is inside this object.
         * \return \c true if the focus is inside this object, \c false otherwise.
         */
        bool isFocusInside() const;
        
    public slots:
        
        /*!
         * Sets item with the property.
         */
        void setItem (Property* property);
        
        /*!
         * Choice of the combo changed.
         */
        void comboBoxChanged(int newChoice);
        
        /*!
         * Updates the data.
         */
        void updateData();
        
        /*!
         * Clears the south panel.
         */
        void clear();
        
        /*!
         * Enables or disables the moving buttons.
         * \param up \c true if the up button should be enabled, \c false otherwise
         * \param down \c true if the down button should be enabled, \c false otherwise
         */
        void setMovingEnabled(bool up, bool down);
        
    signals:
        
        /*!
         * This signal is emitted if the user pressed the "Up" button to move an item one
         * step up.
         */
        void moveUp();
        
        /*!
         * This signal is emitted if the user pressed the "Down" button to move an item one
         * step down.
         */
        void moveDown();
   
    protected:
        
        /*!
         * Inserts the appropriate AutoText.
         */
        void insertAutoText();
        
    private:
        Property* m_currentProperty;
        QLineEdit* m_keyLineEdit;
        QLineEdit* m_valueLineEdit;
        QComboBox* m_typeCombo;
        QToolButton* m_upButton;
        QToolButton* m_downButton;
        int m_oldComboValue;
};

#endif // SOUTHPANEL_H
