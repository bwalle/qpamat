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
#ifndef DEBUG_H
#define DEBUG_H

#include <QApplication>

class MsgHandler;

class QpamatDebug
{
    public:
        static QpamatDebug *instance();
        static void msgHandler(QtMsgType type, const char *msg);
        static QString typeToString(QtMsgType type);
        static const QString DEFAULT_COMPONENT;

    private:
        QpamatDebug();
        ~QpamatDebug();

    public:
        void message(QtMsgType type, const char *msg);
        void setMessageLevel(QtMsgType level);
        void setFilterComponents(const QStringList &components);
        void redirectConsole();
        void redirectFile(const QString &filename);

    private:
        static QpamatDebug *m_instance;
        MsgHandler *m_msgHandler;
        QtMsgType m_msgLevel;
        QStringList m_components;
};

#endif // DEBUG_H

// vim: set sw=4 ts=4 et:
