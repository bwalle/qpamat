/*
 * Id: $Id: insertcarddialog.h,v 1.1 2004/01/20 21:42:46 bwalle Exp $
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
#ifndef INSERTCARDDIALOG_H
#define INSERTCARDDIALOG_H

#include <qstring.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qlineedit.h>

#include "cardpinvalidator.h"

class InsertCardDialog : public QDialog
{
    Q_OBJECT
    
    public:
        InsertCardDialog(bool pin, QWidget* parent, const char* name);
        
    public:
        QString getPIN() const;
        
    protected slots:
        void pinEditHandler(const QString& text);
        
    private:
        QPushButton*    m_okButton;
        QPushButton*    m_cancelButton;
        QLineEdit*      m_pinEdit;
};


#endif // INSERTCARDDIALOG_H
