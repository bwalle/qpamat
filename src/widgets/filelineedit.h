/*
 * Id: $Id: filelineedit.h,v 1.4 2003/12/06 18:24:11 bwalle Exp $
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
#ifndef FILELINEEDIT_H
#define FILELINEEDIT_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qevent.h>

/*!
 * \brief This class provides a QLineEdit combined with a "..." button.
 *
 * The additional button lets the user choose a file on the hard disk with the file 
 * chooser dialog of Qt.
 *
 * \ingroup widgets
 * \author Bernhard Walle
 * \version $Revision: 1.4 $
 * \date $Date: 2003/12/06 18:24:11 $
 */
class FileLineEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString filter READ getFilter WRITE setFilter )
    Q_PROPERTY( QString content READ getContent WRITE setContent )
    
    public:
        
        /*!
         * Creates a new instance of a FileLineEdit widget.
         * \param parent the parent widget
         * \param name the name of the widget which can be NULL.
         */
        FileLineEdit(QWidget* parent, const char* name = 0);
        
        /*!
         * Sets the content. This must be no valid file but it makes sense.
         * \param content the content string
         */
        void setContent(const QString& content);
        
        /*!
         * This returns the current content. It is not guarenteed that it is a valid file.
         * \return the content of the FileLineEdit widget
         */
        QString getContent() const;
        
        /*!
         * Sets a filter for the dialog.
         * \param filter the filter string
         */
        void setFilter(const QString& filter);
        
        /*!
         * Returns the filter.
         * \return the filter that was set or QString::null if no filter was set.
         */
        QString getFilter() const;
        
    protected slots:
        
        /*!
         * Slot that is called if the user pressed the "..." button. It displays a file dialog
         * and lets the user choose the file.
         */
        void chooseFile();
        
    private:
        QLineEdit* m_lineEdit;
        QString m_filter;
        QToolButton* m_fileDialogButton;
};

#endif // FILELINEEDIT_H
