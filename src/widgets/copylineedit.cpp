/*
 * Id: $Id: copylineedit.cpp,v 1.2 2003/12/17 23:05:05 bwalle Exp $
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
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qdir.h>
#include <qapplication.h>
#include <qclipboard.h>

#include "copylineedit.h"
#include "../../images/copy_16x16.xpm"


// -------------------------------------------------------------------------------------------------
CopyLineEdit::CopyLineEdit(QWidget* parent, const char* name)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent, name)
{
    init(false);
}


// -------------------------------------------------------------------------------------------------
CopyLineEdit::CopyLineEdit(bool hidden, QWidget* parent, const char* name)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent, name)
{
    init(hidden);
}


// -------------------------------------------------------------------------------------------------
void CopyLineEdit::init(bool hidden)
// -------------------------------------------------------------------------------------------------
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this, 0, 2);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setReadOnly(true);
    if (hidden)
    {
        m_lineEdit->setEchoMode(QLineEdit::Password);
    }
    
    // we need a tool button because it has no border and it looks better in this size
    // in Platin style
    m_copyButton = new QToolButton(this);
    m_copyButton->setIconSet(QIconSet(copy_16x16_xpm));
    
    boxLayout->addWidget(m_lineEdit);
    boxLayout->addWidget(m_copyButton);
    
    connect(m_copyButton, SIGNAL(clicked()), SLOT(copyText()));
    m_copyButton->setFocusPolicy(QWidget::TabFocus);
    
    setFocusProxy(m_lineEdit);
}


// -------------------------------------------------------------------------------------------------
void CopyLineEdit::setContent(const QString& content)
// -------------------------------------------------------------------------------------------------
{
    m_lineEdit->setText(content);
}


// -------------------------------------------------------------------------------------------------
QString CopyLineEdit::getContent() const
// -------------------------------------------------------------------------------------------------
{
    return m_lineEdit->text();
}


// -------------------------------------------------------------------------------------------------
bool CopyLineEdit::isHidden() const
// -------------------------------------------------------------------------------------------------
{
    return m_lineEdit->echoMode() == QLineEdit::Password;
}


// -------------------------------------------------------------------------------------------------
void CopyLineEdit::setHidden(bool hidden)
// -------------------------------------------------------------------------------------------------
{
    m_lineEdit->setEchoMode(hidden ? QLineEdit::Password : QLineEdit::Normal);
}


// -------------------------------------------------------------------------------------------------
void CopyLineEdit::copyText()
// -------------------------------------------------------------------------------------------------
{
    QClipboard* cb = QApplication::clipboard();
    cb->setText(m_lineEdit->text(), QClipboard::Clipboard);
    cb->setText(m_lineEdit->text(), QClipboard::Selection);
}


