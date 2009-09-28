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
#ifndef QPAMAT_H
#define QPAMAT_H

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

#include <QString>

class QpamatWindow;

class Qpamat : boost::noncopyable
{
    public:
        static Qpamat *instance();

        void installTranslations();
        void registerDBus();
        void parseCommandLine(int argc, char **argv);
        void printCommandlineOptions();

        QpamatWindow *getWindow();

    protected:
        void printVersion();
        void getX11Version(QString &protocolVersion, QString &vendorVersion);

    private:
        Qpamat();

    private:
        static Qpamat *m_instance;

    private:
        boost::scoped_ptr<QpamatWindow> m_qpamatWindow;
};

#endif // QPAMAT_H

// vim: set sw=4 ts=4 et:
