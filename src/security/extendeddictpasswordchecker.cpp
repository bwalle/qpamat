/*
 * Id: $Id: extendeddictpasswordchecker.cpp,v 1.1 2003/12/04 11:56:05 bwalle Exp $
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
#include <stdexcept>

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qapplication.h>
#include <qcursor.h>

#include "extendeddictpasswordchecker.h"

// -------------------------------------------------------------------------------------------------
ExtendedDictPasswordChecker::ExtendedDictPasswordChecker(const QString& file)
// -------------------------------------------------------------------------------------------------
         throw (std::invalid_argument)
{
    if (file != QString::null && !QFile::exists(file))
    {
        throw std::invalid_argument(("The specified file '" + file + "' does not exist.").latin1());
    }
    else
    {
        m_file = file;
    }
}

// -------------------------------------------------------------------------------------------------
bool ExtendedDictPasswordChecker::isPasswordOk(const QString& password)
// -------------------------------------------------------------------------------------------------
            throw (PasswordCheckException)
{
    if (!ExtendedPasswordChecker::isPasswordOk(password))
    {
        return false;
    }
    
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    
    if (m_file != QString::null)
    {
        QFile file(m_file);
        if (!file.open(IO_ReadOnly | IO_Translate))
        {
            QApplication::restoreOverrideCursor();
            throw PasswordCheckException( QString("Could not open the file \"%s\".")
                .arg(m_file).latin1());
        }
        QTextStream stream(&file);
        QString line = QString::null;
        while ((line = stream.readLine()) != QString::null)
        {
            line = line.stripWhiteSpace();
            if (password.contains(line) && line.length() >= 2)
            {
                QApplication::restoreOverrideCursor();
                return false;
            }
        }
    }
    
    QApplication::restoreOverrideCursor();
    
    return true;
}

