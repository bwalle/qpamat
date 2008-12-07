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
#include <QString>
#include <QPixmap>
#include <QLayout>
#include <QLabel>
#include <QHBoxLayout>

#include "waitdialog.h"


/*!
    \class WaitDialog
    
    \brief Displays a dialog which a simple icon and a message.
    
    This is a dialog used to tell the user that something is done and he should wait. This is where 
    a progress dialog makes no or little sense because we don't know exactly how long it takes.
    Of course, the dialog is modal.
    
    \code
  QIconSet icon(myicon_xpm);
  WaitDialog* dlg = new WaitDialog(icon, tr("Message"), "QPaMaT", this, "Wait Dialog");
  dlg->exec();
  // do something
  delete dlg;
    \endcode
    
    \ingroup dialogs
    \author Bernhard Walle
*/

/*!
    Creates a new instance of a wait dialog.
    \param icon the icon that should be displayed (use a null pixmap if there should beno icon
    \param message the message text that should be displayed
    \param title the title
    \param parent the parent widget
    \param name the name of the widget
*/
WaitDialog::WaitDialog(const QPixmap& icon, const QString& message, const QString& title, 
                       QWidget* parent, const char* name)
    : QDialog(parent, name, true)
{
    setCaption(title);
    QHBoxLayout* box = new QHBoxLayout(this,  5, 5, "WaitDialog-Hbox");
    
    QLabel* imageLabel = 0;
    
    if (!icon.isNull())
    {
        imageLabel = new QLabel(this, "WaitDialog-Label");
        imageLabel->setPixmap(icon);
    }
    
    QLabel* textLabel = new QLabel(message, this, "WaitDialog-TextLabel");
    textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter | Qt::ExpandTabs);
    
    if (!icon.isNull())
    {
        box->addWidget(imageLabel);
        
    }
    box->addSpacing(10);
    box->addWidget(textLabel);
    box->addSpacing(10);
    if (!icon.isNull())
    {
        box->setStretchFactor(imageLabel, 0);
    }
    box->setStretchFactor(textLabel, 5);
}

// :maxLineLen=100:
