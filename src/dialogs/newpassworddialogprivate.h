/*
 * Id: $Id: newpassworddialogprivate.h,v 1.1 2003/12/28 22:08:15 bwalle Exp $
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
#ifndef NEWPASSWORDDIALOGPRIVATE_H
#define NEWPASSWORDDIALOGPRIVATE_H

#include <qvalidator.h>
#include <qstring.h>

class PasswordValidator : public QValidator
{
    public:
        PasswordValidator(QObject* parent = 0, const char* name = 0);
        State validate(QString& input, int& pos) const;
};

#endif // NEWPASSWORDDIALOGPRIVATE_H
