/*
 * Id: $Id: southpanel.h,v 1.1 2003/10/20 20:55:13 bwalle Exp $
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

#include "property.h"

/**
 * Description
 * @ingroup gui
 * @author Bernhard Walle
 * @version $Revision: 1.1 $
 * @date $Date: 2003/10/20 20:55:13 $
 */
class SouthPanel : public QFrame
{
    Q_OBJECT
    
    public:
        SouthPanel(QWidget* parent);
        virtual ~SouthPanel() { }
        
    public slots:
        void setItem (Property* property);
        void updateData();
        void clear();
   
    protected:
        //void focusInEvent(QFocusEvent* evt);
        
    private:
        Property* m_currentProperty;
        QLineEdit* m_keyLineEdit;
        QLineEdit* m_valueLineEdit;
        QComboBox* m_typeCombo;
};

#endif // SOUTHPANEL_H
