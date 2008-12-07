/*
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
#include <QLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QPixmap>
#include <QDir>
#include <QHBoxLayout>

#include "filelineedit.h"

/*!
    \class FileLineEdit
    
    \brief This class provides a QLineEdit combined with a "..." button.

    The additional button lets the user choose a file on the hard disk with the file 
    chooser dialog of Qt.
    
    \ingroup widgets
    \author Bernhard Walle
    \version $Revision: 1.9 $
    \date $Date$
*/

/*!
    Creates a new instance of a FileLineEdit widget.
    \param parent the parent widget
    \param save \c true if the file dialog should be a "save dialog" (file must not exist),
                \c false if it should be a "open dialog" (file must exist)
    \param name the name of the widget which can be NULL.
*/
FileLineEdit::FileLineEdit(QWidget* parent, bool save, const char* name)
    : QWidget(parent, name), m_save(save)
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this, 0, 2);
    m_lineEdit = new QLineEdit(this);
    
    // we need a tool button because it has no border and it looks better in this size
    // in Platin style
    m_fileDialogButton = new QToolButton(this);
    m_fileDialogButton->setIconSet(QIcon(QPixmap(":/images/stock_directory_16.png")));
    
    boxLayout->addWidget(m_lineEdit);
    boxLayout->addWidget(m_fileDialogButton);
    
    connect(m_fileDialogButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    m_fileDialogButton->setFocusPolicy(Qt::TabFocus);
    
    setFocusProxy(m_lineEdit);
    
}

/*!
    \property FileLineEdit::content
    
    The content, i. e. the current file, of the line edit.
*/

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
    \property FileLineEdit::filter
    
    The current filter.
*/

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
    
    QString selected;
    if (m_save)
    {
        selected = QFileDialog::getSaveFileName (startWith, m_filter, this, "File Dialog",
            tr("QPaMaT"));
    }
    else
    {
        selected = QFileDialog::getOpenFileName (startWith, m_filter, this, "File Dialog",
            tr("QPaMaT"));
    }
        
    if (selected != QString::null)
    {
        m_lineEdit->setText(selected);
    }
}

// :maxLineLen=100:
