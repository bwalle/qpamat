/*
 * Id: $Id: fontchoosebox.h,v 1.2 2003/12/10 21:44:16 bwalle Exp $
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
#ifndef FONTCHOOSEBOX_H
#define FONTCHOOSEBOX_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qfont.h>

/*!
 * \brief This class provides a not-editable QLineEdit combined with a button.
 *
 * The additional button lets the user choose a font which is available on the system.
 *
 * \ingroup widgets
 * \author Bernhard Walle
 * \version $Revision: 1.2 $
 * \date $Date: 2003/12/10 21:44:16 $
 */
class FontChooseBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QFont font READ getFont WRITE setFont )
    
    public:
        
        /*!
         * Creates a new instance of a FontChooseBox widget.
         * \param parent the parent widget
         * \param name the name of the widget which can be NULL.
         */
        FontChooseBox(QWidget* parent, const char* name = 0);
        
        /*!
         * Sets the font. The font is displayed in itself.
         * \param font content the content string
         */
        void setFont(const QFont& font);
        
        /*!
         * This returns the current font.
         * \return the font
         */
        QFont getFont() const;
    
    signals:
        
        /*!
         * This signal is emitted if the font has changed.
         * \param font the new font
         */
        void fontChanged(const QFont& font);
        
    protected slots:
        
        /*!
         * Slot that is called if the user pressed the button. It displays a QFontDialog
         * and lets the user choose the font.
         */
        void chooseFont();
        
        /*!
         * Sets the font to display the line edit
         * \param font the font object
         */
        void setDisplayFont(const QFont& font);
        
    private:
        QLineEdit* m_lineEdit;
        QToolButton* m_fileDialogButton;
        QFont m_font;
};

#endif // FONTCHOOSEBOX_H
