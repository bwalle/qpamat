/*
 * Id: $Id: randompassword.h,v 1.3 2003/12/29 15:12:27 bwalle Exp $
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
#ifndef RANDOMPASSWORD_H
#define RANDOMPASSWORD_H

#include <qobject.h>
#include <qstring.h>

class Qpamat;

class RandomPassword : public QObject
{
    Q_OBJECT
    
    public:
        RandomPassword(Qpamat* parent, const char* name = 0);
    
    public slots:
        void setInsertEnabled(bool enabled = true);
        void requestPassword();
        
    signals:
        void insertPassword(const QString& password);
        
    private:
        bool m_insertEnabled;
        Qpamat* m_parent;
};

#endif // RANDOMPASSWORD_H
