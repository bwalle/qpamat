/*
 * Id: $Id: help.cpp,v 1.4 2003/12/20 15:58:02 bwalle Exp $
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

// -------------------------------------------------------------------------------------------------
void Help::showAbout()
// -------------------------------------------------------------------------------------------------
{
    QMessageBox::about(qApp->mainWidget(), "QPaMaT",
        tr("<p><b>QPaMaT "VERSION"</b></p><p>Password managing tool for Unix, Windows and MacOS X. "
            "It uses the OpenSSL library for encryption and is distributed under the terms "
            "of the GNU General Public License.</p>"
            "<p>© Bernhard Walle &lt;bernhard.walle@gmx.de&gt;"));
}


// -------------------------------------------------------------------------------------------------
void Help::openURL(QWidget* parent, const QString& url)
// -------------------------------------------------------------------------------------------------
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
