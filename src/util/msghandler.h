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
#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <QString>
#include <QFile>

/* MsgHandler {{{ */

class MsgHandler
{
    public:
        ~MsgHandler() {}

    public:
        virtual bool open() = 0;
        virtual void output(QtMsgType       type,
                            const QString   &context,
                            const QString   &date,
                            const QString   &component,
                            const QString   &msg) = 0;
};

/* }}} */
/* FileMsgHandler {{{ */

class FileMsgHandler : public MsgHandler
{
    public:
        FileMsgHandler(const char *filename);
        ~FileMsgHandler();

    public:
        bool open();
        void output(QtMsgType       type,
                    const QString   &context,
                    const QString   &date,
                    const QString   &component,
                    const QString   &msg);

    private:
        QFile m_outputfile;
};

/* }}} */
/* StderrMsgHandler {{{ */

class StderrMsgHandler : public MsgHandler
{
    public:
        StderrMsgHandler();
        ~StderrMsgHandler();

    public:
        bool open();
        void output(QtMsgType       type,
                    const QString   &context,
                    const QString   &date,
                    const QString   &component,
                    const QString   &msg);

    private:
        bool m_useColor;
};

/* }}} */

#endif // MSGHANDLER_H

// vim: set sw=4 ts=4 et:
