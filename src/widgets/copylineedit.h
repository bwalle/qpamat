/*
 * Id: $Id: copylineedit.h,v 1.1 2003/12/17 21:56:35 bwalle Exp $
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
#ifndef COPYLINEEDIT_H
#define COPYLINEEDIT_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qevent.h>

/*!
 * \brief This class provides a QLineEdit combined with a copy button.
 *
 * The copy buttons lets the user copy the displayed text.
 * chooser dialog of Qt.
 *
 * \ingroup widgets
 * \author Bernhard Walle
 * \version $Revision: 1.1 $
 * \date $Date: 2003/12/17 21:56:35 $
 */
class CopyLineEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString content READ getContent WRITE setContent )
    
    public:
        
        /*!
         * Creates a new instance of a FileLineEdit widget.
         * \param parent the parent widget
         * \param name the name of the widget which can be NULL.
         */
        CopyLineEdit(QWidget* parent, const char* name = 0);
        
        /*!
         * Sets the content.
         * \param content the content string
         */
        void setContent(const QString& content);
        
        /*!
         * This returns the current content.
         * \return the content of the CopyLineEdit widget
         */
        QString getContent() const;
        
    protected slots:
        
        /*!
         * Slot that is called if the user pressed the copy button. It copies the displayed
         * text in the clipboard (and primary selection, on X11).
         */
        void copyText();
        
    private:
        QLineEdit* m_lineEdit;
        QToolButton* m_copyButton;
};

#endif // COPYLINEEDIT_H
