/*
 * Id: $Id: help.cpp,v 1.7 2004/07/23 08:47:58 bwalle Exp $
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
#include <qobject.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qprocess.h>

#include "qpamat.h"
#include "help.h"
#include "settings.h"
#include "dialogs/aboutdialog.h"

/*!
    \class Help
    
    \brief Helping class for providing help.
    
    \ingroup gui
    \author Bernhard Walle
    \version $Revision: 1.7 $
    \date $Date: 2004/07/23 08:47:58 $
*/

/*!
    Shows the about dialog
*/
void Help::showAbout()
{
    std::auto_ptr<AboutDialog> dlg(new AboutDialog(qApp->mainWidget(), "About Dialog"));
    
    dlg->exec();
}


/*!
    Opens the specified URL in the browser.
*/
void Help::openURL(QWidget* parent, const QString& url)
{
    QString command = qpamat->set().readEntry("/General/Webbrowser");
    QProcess* process = new QProcess(command, parent);
    process->addArgument(url);
    
    if (! process->start())
    {
        QMessageBox::critical(parent, "QPaMaT",
            tr("<qt><p>Failed to open the link <tt>%1</tt> in the specified web browser."
                " The command was:</p><p><tt><nobr>%2</tt></nobr></p></qt>").arg(url).arg(
                command+" " + url), QMessageBox::Ok, QMessageBox::NoButton);
    }
}
