/*
 * Id: $Id: filelineedit.h,v 1.6 2004/02/07 00:11:04 bwalle Exp $
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

class FileLineEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString filter READ getFilter WRITE setFilter )
    Q_PROPERTY( QString content READ getContent WRITE setContent )
    
    public:
        FileLineEdit(QWidget* parent, bool save, const char* name = 0);
        
        void setContent(const QString& content);
        QString getContent() const;
        
        void setFilter(const QString& filter);
        QString getFilter() const;
        
    protected slots:
        void chooseFile();
        
    private:
        QLineEdit*      m_lineEdit;
        QString         m_filter;
        QToolButton*    m_fileDialogButton;
        bool            m_save;
};

#endif // FILELINEEDIT_H
