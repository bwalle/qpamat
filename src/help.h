/*
 * Id: $Id: help.h,v 1.3 2003/11/29 14:43:03 bwalle Exp $
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
#ifndef HELP_H
#define HELP_H

#include <qobject.h>
#include <qwidget.h>

/*!
 * \brief Helping class for providing help.
 * \ingroup gui
 * \author Bernhard Walle
 * \version $Revision: 1.3 $
 * \date $Date: 2003/11/29 14:43:03 $
 */
class Help : public QObject
{
    Q_OBJECT
    
    public slots:
        
        /*!
         * Shows the about dialog
         */
        void showAbout();
        
        /*!
         * Opens the specified URL in the browser.
         */
        static void openURL(QWidget* parent, const QString& url);
        
};

#endif // HELP_H
