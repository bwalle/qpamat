/*
 * Id: $Id: randompassword.cpp,v 1.3 2003/12/18 21:59:47 bwalle Exp $
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
#include <qstring.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qcursor.h>

#include "qpamat.h"
#include "settings.h"
#include "security/configpasswordchecker.h"
#include "dialogs/randompassworddialog.h"
#include "randompassword.h"
#include "security/passwordgeneratorfactory.h"


// -------------------------------------------------------------------------------------------------
RandomPassword::RandomPassword(Qpamat* parent, const char* name)
// -------------------------------------------------------------------------------------------------
            : QObject(parent, name), m_insertEnabled(false), m_parent(parent)
{
}


// -------------------------------------------------------------------------------------------------
void RandomPassword::setInsertEnabled(bool enabled)
// -------------------------------------------------------------------------------------------------
{
    m_insertEnabled = enabled;
}


// -------------------------------------------------------------------------------------------------
void RandomPassword::requestPassword()
// -------------------------------------------------------------------------------------------------
{
    QSettings& set = Settings::getInstance().getSettings();
    RandomPasswordDialog* dlg = new RandomPasswordDialog(
        m_parent, m_insertEnabled, "RandomPwDialog");
    
    PasswordGenerator* passwordgen = 0;
    int length = set.readNumEntry("Security/Length", PasswordGeneratorFactory::DEFAULT_LENGTH);
    QString ensured = set.readEntry("Security/EnsuredCharacters", "ULDs");
    QString allowed = set.readEntry("Security/AllowedCharacters", "a-zA-Z0-9");
    ConfigPasswordChecker checker(length, ensured, allowed);
    
    try
    {
        passwordgen = PasswordGeneratorFactory::getGenerator(
            set.readEntry( "Security/PasswordGenerator", 
                PasswordGeneratorFactory::DEFAULT_GENERATOR_STRING ),
            set.readEntry( "Security/PasswordGeneratorAdditional" ) 
        );
    }
    catch (const std::exception& exc)
    {
        QMessageBox::warning(m_parent, "QPaMaT",
                tr("<qt><nobr>Failed to create a password checker:</nobr><br>%1</qt>")
                .arg(exc.what()), QMessageBox::Ok, QMessageBox::NoButton);
        delete passwordgen;
        delete dlg;
        return;
    }
    
    if (passwordgen->isSlow())
    {
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    }
    
    bool ok = false;
    QString password;
    for (int i = 0; !ok && i < 200; ++i)
    {
        try
        {
            password = passwordgen->getPassword(
                set.readNumEntry("Security/Length", 
                PasswordGeneratorFactory::DEFAULT_LENGTH ),
                checker.allowedCharacters()
            );
            ok = checker.isPasswordOk(password);
        }
        catch (const std::exception& exc)
        {
            if (passwordgen->isSlow())
            {
                QApplication::restoreOverrideCursor();
            }
            QMessageBox::warning(m_parent, "QPaMaT",
                tr("<qt><nobr>An error occurred while generating the password:</nobr><br>%1</qt>")
                .arg(exc.what()), QMessageBox::Ok, QMessageBox::NoButton);
            break;
        }
    }
    
    if (passwordgen->isSlow())
    {
        QApplication::restoreOverrideCursor();
    }
    
    if (ok)
    {
        dlg->setPassword(password);
        connect(dlg, SIGNAL(insertPassword(const QString&)), SIGNAL(insertPassword(const QString&)));
        dlg->exec();
    }
    else
    {
        QMessageBox::information(m_parent, "QPaMaT", tr("<qt><nobr>Failed to create a random "
            "password with the current</nobr> generator and the current password checker. "
            "Adjust the settings!"), QMessageBox::Ok, QMessageBox::NoButton);
    }
    delete dlg;
}

