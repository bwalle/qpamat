/*
 * Id: $Id: filelineedit.cpp,v 1.6 2003/12/29 00:37:11 bwalle Exp $
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
#include <qpixmap.h>
#include <qdir.h>

#include "filelineedit.h"
#include "images/find_16x16.xpm"

/*!
    \class FileLineEdit
    
    \brief This class provides a QLineEdit combined with a "..." button.

    The additional button lets the user choose a file on the hard disk with the file 
    chooser dialog of Qt.
    
    \ingroup widgets
    \author Bernhard Walle
    \version $Revision: 1.6 $
    \date $Date: 2003/12/29 00:37:11 $
*/

/*!
    Creates a new instance of a FileLineEdit widget.
    \param parent the parent widget
    \param name the name of the widget which can be NULL.
*/
FileLineEdit::FileLineEdit(QWidget* parent, const char* name)
    : QWidget(parent, name)
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this, 0, 2);
    m_lineEdit = new QLineEdit(this);
    
    // we need a tool button because it has no border and it looks better in this size
    // in Platin style
    m_fileDialogButton = new QToolButton(this);
    m_fileDialogButton->setIconSet(QIconSet(find_16x16_xpm));
    
    boxLayout->addWidget(m_lineEdit);
    boxLayout->addWidget(m_fileDialogButton);
    
    connect(m_fileDialogButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    m_fileDialogButton->setFocusPolicy(QWidget::TabFocus);
    
    setFocusProxy(m_lineEdit);
    
}


/*!
    Sets the content. This must be no valid file but it makes sense.
    \param content the content string
*/
void FileLineEdit::setContent(const QString& content)
{
    m_lineEdit->setText(content);
}


/*!
    This returns the current content. It is not guarenteed that it is a valid file.
    \return the content of the FileLineEdit widget
*/
QString FileLineEdit::getContent() const
{
    return m_lineEdit->text();
}


/*!
     Sets a filter for the dialog.
     \param filter the filter string
*/
void FileLineEdit::setFilter(const QString& filter)
{
    m_filter = filter;
}


/*!
    Returns the filter.
    \return the filter that was set or QString::null if no filter was set.
*/
QString FileLineEdit::getFilter() const
{
    return m_filter;
}


/*!
    Slot that is called if the user pressed the "..." button. It displays a file dialog
    and lets the user choose the file.
*/
void FileLineEdit::chooseFile()
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


