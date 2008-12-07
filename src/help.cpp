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
#include <memory>

#include <QObject>
#include <QApplication>
#include <QMessageBox>
#include <QWidget>
#include <Q3Process>
#include <QTextCodec>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>

#include "global.h"
#include "qpamat.h"
#include "help.h"
#include "settings.h"
#include "dialogs/aboutdialog.h"

/*!
    \class Help
    
    \brief Helping class for providing help.
    
    \ingroup gui
    \author Bernhard Walle
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
    Shows the documentation in the browser specified by the user.
*/
void Help::showHelp()
{
    QString base = QDir(qApp->applicationDirPath() + "/../share/qpamat/doc/").canonicalPath();
    QString loc = QString(QTextCodec::locale()).section("_", 0, 0);
    
    if (QFile::exists(base + "/" + loc + "/index.html"))
    {
        openURL(qApp->mainWidget(), "file:///" + base + "/" + loc + "/index.html");
    }
    else if (QFile::exists(base + "/en/index.html"))
    {
        openURL(qApp->mainWidget(), "file:///" + base + "/en/index.html");
    }
    else
    {
        QMessageBox::critical(qApp->mainWidget(), tr("QPaMaT"), 
            tr("The QPaMaT documentation is not installed."), QMessageBox::Ok, 
            QMessageBox::NoButton );
        return;
    }
    
    qpamat->message(tr("Help started in web browser."));
}


/*!
    Opens the specified URL in the browser.
*/
void Help::openURL(QWidget* parent, const QString& url)
{
    if (!QDesktopServices::openUrl(QUrl(url)))
    {
        QMessageBox::critical(parent, "QPaMaT",
            tr("<qt><p>Failed to open the link <tt>%1</tt> in "
                "the specified web browser.</p></qt>").arg(url), 
            QMessageBox::Ok, QMessageBox::NoButton);
    }
}

// :maxLineLen=100:
