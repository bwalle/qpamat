/*
 * Id: $Id: fontchoosebox.cpp,v 1.1 2003/12/06 18:24:16 bwalle Exp $
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
#include <qlayout.h>
#include <qfont.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qfontdialog.h>

#include "fontchoosebox.h"
#include "../../images/font_16x16.xpm"

// -------------------------------------------------------------------------------------------------
FontChooseBox::FontChooseBox(QWidget* parent, const char* name)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent, name)
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this, 0, 2);
    m_lineEdit = new QLineEdit(this);
    
    // we need a tool button because it has no border and it looks better in this size
    // in Platin style
    m_fileDialogButton = new QToolButton(this);
    m_fileDialogButton->setIconSet(QIconSet(font_16x16_xpm));
    
    boxLayout->addWidget(m_lineEdit);
    boxLayout->addWidget(m_fileDialogButton);
    
    connect(m_fileDialogButton, SIGNAL(clicked()), this, SLOT(chooseFont()));
    
    setFocusProxy(m_lineEdit);
    
    // make it read-only
    m_lineEdit->setReadOnly(true);
    
    // connect
    connect(this, SIGNAL(fontChanged(const QFont&)), this, SLOT(setDisplayFont(const QFont&)));
}


// -------------------------------------------------------------------------------------------------
void FontChooseBox::setFont(const QFont& font)
// -------------------------------------------------------------------------------------------------
{
    m_font = font;
    emit fontChanged(m_font);
}


// -------------------------------------------------------------------------------------------------
QFont FontChooseBox::getFont() const
// -------------------------------------------------------------------------------------------------
{
    return m_font;
}


// -------------------------------------------------------------------------------------------------
void FontChooseBox::setDisplayFont(const QFont& font)
// -------------------------------------------------------------------------------------------------
{
    QFont displayFont(font.family(), QApplication::font().pointSize());
    m_lineEdit->setFont(displayFont);
    m_lineEdit->setText(font.family() + " " + QString::number(font.pointSize()));
}


// -------------------------------------------------------------------------------------------------
void FontChooseBox::chooseFont()
// -------------------------------------------------------------------------------------------------
{
     bool ok;
     QFont font = QFontDialog::getFont(&ok, m_font, this );
     
     if ( ok ) 
     {
         setFont(font);
     }
}


