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
#include <iostream>

#include <QTextStream>

#include "msghandler.h"
#include "ansicolor.h"
#include "platformhelpers.h"
#include "debug.h"

/**
 * @class MsgHandler
 *
 * @brief Interface to implement message handlers
 *
 * Don't use that interface and the derived class directly in the application.
 * Instead, use the functions provided by QpamatDebug, especially
 * QpamatDebug::redirectConsole() and QpamatDebug::redirectFile().
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup misc
 */

/**
 * @fn MsgHandler::open()
 *
 * @brief Opens the underlying device for a message handler.
 *
 * Before messages can be written with MsgHandler::output(), this function must
 * be called. The message handler can only be used if the function returns
 * @c true.
 *
 * @return @c true if the underlying device successfully was opened, @c false
 *         otherwise.
 */

/**
 * @fn MsgHandler::output(QtMsgType, const QString &, const QString &, const QString &, const QString &)
 *
 * @brief Output function for the message handler.
 *
 * This function prints the message to the underlying device. Filtering is
 * already done.
 *
 * @param[in] type the severity of the message handler
 * @param[in] context the function name and/or line number from where the
 *            message has been printed
 * @param[in] date the date (already formatted)
 * @param[in] component the component of the message handler (can be
 *            QpamatDebug::DEFAULT_COMPONENT) but cannot be QString::null.
 * @param[in] msg the real message
 */

/* StderrMsgHandler {{{ */

/**
 * @class StderrMsgHandler
 * @brief Creates a new instance of StderrMsgHandler
 *
 * The stderr message handler logs on standard error output. On POSIX platforms,
 * if standard error is not redirected to a file, the logging messages are
 * printed in color.
 *
 * In the application, don't use that class directly. Instead, use
 * QpamatDebug::redirectConsole().
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup misc
 */


/** 
 * @brief Constructor
 *
 * Creates a new instance of StderrMsgHandler.
 */
StderrMsgHandler::StderrMsgHandler()
    : m_useColor(PlatformHelpers::isTerminal(PlatformHelpers::FC_STDERR))
{}


/** 
 * @brief Destructor
 */
StderrMsgHandler::~StderrMsgHandler()
{}


/// @copydoc MsgHandler::open()
bool StderrMsgHandler::open()
{
    return true;
}


/// @copydoc MsgHandler::output()
void StderrMsgHandler::output(QtMsgType       type,
                              const QString   &context,
                              const QString   &date,
                              const QString   &component,
                              const QString   &msg)
{
    const char *colorcode = "", *endcolorcode = "";

    // color string
    if (m_useColor) {
        switch (type) {
            case QtDebugMsg:
                colorcode = AnsiColor::YELLOW;
                break;
            case QtWarningMsg:
                colorcode = AnsiColor::MAGENTA;
                break;
            case QtCriticalMsg:
                colorcode = AnsiColor::RED;
                break;
            case QtFatalMsg:
                colorcode = AnsiColor::BRIGHT_RED;
                break;
        }
        endcolorcode = AnsiColor::NORMAL;
    }

    QString typeString = "[" + QpamatDebug::typeToString(type) + "]";

    QString outputstring;
    QTextStream stream(&outputstring, QIODevice::WriteOnly);

    // date
    stream.setFieldAlignment(QTextStream::AlignLeft);
    stream.setFieldWidth(21);
    stream << date;

    // type
    stream.setFieldWidth(11);
    stream << typeString;

    // component
    stream.setFieldWidth(15);
    stream << component;

    // context and msg
    stream.setFieldWidth(0);
    if (!context.isNull()) {
        stream << context << " ||| ";
    }
    stream << msg;

    std::cerr << colorcode
              << outputstring.toLocal8Bit().constData()
              << endcolorcode
              << std::endl;
}

/* }}} */
/* FileMsgHandler {{{ */

/**
 * @class FileMsgHandler
 * @brief Creates a new instance of FileMsgHandler
 *
 * The file message handler logs in a file. The file will not be truncated,
 * instead, log messages are appended.
 *
 * In the application, don't use that class directly. Instead, use
 * QpamatDebug::redirectFile().
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup misc
 */


/**
 * @brief Constructor
 *
 * Creates a new instance of FileMsgHandler.
 *
 * @param[in] outputfile the name of the outputfile
 */
FileMsgHandler::FileMsgHandler(const char *outputfile)
    : m_outputfile(outputfile)
{}

/// Destructor
FileMsgHandler::~FileMsgHandler()
{
    m_outputfile.close();
}


/// @copydoc MsgHandler::open()
bool FileMsgHandler::open()
{
    return m_outputfile.open(QIODevice::WriteOnly |
                             QIODevice::Append |
                             QIODevice::Text |
                             QIODevice::Unbuffered);

}


/// @copydoc MsgHandler::output()
void FileMsgHandler::output(QtMsgType       type,
                            const QString   &context,
                            const QString   &date,
                            const QString   &component,
                            const QString   &msg)
{
    QString typeString = "[" + QpamatDebug::typeToString(type) + "]";

    QTextStream stream(&m_outputfile);

    // date
    stream.setFieldAlignment(QTextStream::AlignLeft);
    stream.setFieldWidth(21);
    stream << date;

    // type
    stream.setFieldWidth(11);
    stream << typeString;

    // component
    stream.setFieldWidth(15);
    stream << component;

    // context and msg
    stream.setFieldWidth(0);
    if (!context.isNull()) {
        stream << context << " ||| ";\
    }
    stream << msg;
    stream << endl;
}

/* }}} */

// vim: set sw=4 ts=4 et:
