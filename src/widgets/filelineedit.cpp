/*
 * Id: $Id: filelineedit.cpp,v 1.1 2003/11/16 12:10:28 bwalle Exp $
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
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qdir.h>

#include "filelineedit.h"

// -------------------------------------------------------------------------------------------------
FileLineEdit::FileLineEdit(QWidget* parent, const char* name)
// -------------------------------------------------------------------------------------------------
        : QWidget(parent, name)
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this);
    m_lineEdit = new QLineEdit(this);
    m_fileDialogButton = new QPushButton("...", this);
    
    // this is ok here :-)
    m_fileDialogButton->setFixedWidth(30);
    
    boxLayout->addWidget(m_lineEdit);
    boxLayout->addWidget(m_fileDialogButton);
    
    connect(m_fileDialogButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    
    setFocusProxy(m_lineEdit);
    
}


// -------------------------------------------------------------------------------------------------
void FileLineEdit::setContent(const QString& content)
// -------------------------------------------------------------------------------------------------
{
    m_lineEdit->setText(content);
}


// -------------------------------------------------------------------------------------------------
QString FileLineEdit::getContent()
// -------------------------------------------------------------------------------------------------
{
    return m_lineEdit->text();
}


// -------------------------------------------------------------------------------------------------
void FileLineEdit::setFilter(const QString& filter)
// -------------------------------------------------------------------------------------------------
{
    m_filter = filter;
}


// -------------------------------------------------------------------------------------------------
void FileLineEdit::chooseFile()
// -------------------------------------------------------------------------------------------------
{
    QString startWith = m_lineEdit->text();
    if (startWith.length() == 0)
    {
        startWith = QDir::homeDirPath();
    }
    
    QString selected = QFileDialog::getOpenFileName (startWith, m_filter, this, "File Dialog",
        tr("QPaMaT"));
        
    if (selected != QString::null)
    {
        m_lineEdit->setText(selected);
    }
}


