/*
 * Id: $Id: randompassword.cpp,v 1.1 2003/12/16 22:51:51 bwalle Exp $
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

#include "dialogs/randompassworddialog.h"
#include "randompassword.h"
#include "security/passwordgeneratorfactory.h"


// -------------------------------------------------------------------------------------------------
RandomPassword::RandomPassword(QWidget* parent, const char* name)
// -------------------------------------------------------------------------------------------------
            : QObject(parent, name), m_insertEnabled(false)
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
    RandomPasswordDialog* dlg = new RandomPasswordDialog(
        dynamic_cast<QWidget*>(parent()), m_insertEnabled, "RandomPwDialog");
    
    PasswordGenerator* passwordgen = PasswordGeneratorFactory::getGenerator("RANDOM");
    dlg->setPassword(passwordgen->getPassword(8));
    connect(dlg, SIGNAL(insertPassword(const QString&)), SIGNAL(insertPassword(const QString&)));
    dlg->exec();
    delete dlg;
}

