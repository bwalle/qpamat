/*
 * Id: $Id: focuslineedit.h,v 1.1 2003/12/17 21:56:43 bwalle Exp $
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
#ifndef FOCUSLINEEDIT_H
#define FOCUSLINEEDIT_H

#include <qlineedit.h>
#include <qevent.h>
#include <qstring.h>
#include <qwidget.h>

/**
 * \brief Implements a QLineEdit which notifies the user when it gets the focus.
 *
 * This FocusLineEdit is exactly the same as a normal QLineEdit but it has an additional signal
 * gotFocus() which notifies the user of this widget to do something if the element got the focus.
 *
 * \ingroup widgets
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/17 21:56:43 $
 */
class FocusLineEdit : public QLineEdit
{
    Q_OBJECT
    
    public:
        
        /*!
         * Constructor passed to QLineEdit.
         * \param parent the parent
         * \param name the name
         */
        FocusLineEdit(QWidget* parent, const char* name = 0);
        
        /*!
         * Constructor passed to QLineEdit.
         * \param contents initial content
         * \param parent the parent
         * \param name the name
         */
         FocusLineEdit(const QString& contents, QWidget* parent, const char* name = 0);
         
         /*!
          * Constructor passed to QLineEdit.
          * \param contents initial content
          * \param inputMask the input mask
          * \param parent the parent
          * \param name the name
          */
         FocusLineEdit(const QString& contents, const QString& inputMask, QWidget* parent,
             const char* name = 0);
             
     protected:
        
        /*!
         * Reimplemented focusInEvent function to provide the gotFocus signal.
         */
        void focusInEvent(QFocusEvent* evt);
        
     signals:
        
        /*!
         * This signal is emitted if the widget got the focus.
         */
        void gotFocus();
};

#endif // FOCUSLINEEDIT_H
