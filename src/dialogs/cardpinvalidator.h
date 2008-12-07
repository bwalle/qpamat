/*
 * Id: $Id$
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
#ifndef CARDPINVALIDATOR_H
#define CARDPINVALIDATOR_H

#include <QValidator>
#include <QObject>
#include <QString>

class CardPINValidator : public QValidator
{
    public:
        CardPINValidator(QObject* parent, const char* name = 0);
        
    public:
        State validate(QString& input, int& pos) const;
};

#endif // CARDPINVALIDATOR_H

// :maxLineLen=100:
