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
#include <QDateTime>
#include <QFile>

#include "platformhelpers.h"
#include "debug.h"
#include "msghandler.h"

/**
 * @class QpamatDebug
 *
 * @brief Logging/Debugging implementation for Qpamat
 *
 * This class provides an implementation for a Qt message handler. It provides
 * the additional functionality of filtering (both for a component and the
 * severity).
 *
 * To use it in the application, you have to install a message handler:
 *
 * @code
 * qInstallMsgHandler(QpamatDebug::msgHandler);
 * @endcode
 *
 * You can always configure the log handler. To also print out debugging
 * message, use: 
 *
 * @code
 * QpamatDebug::instance()->setMessageLevel(QtDebugMsg);
 * @endcode
 *
 * To filter out all components beside the default component:
 *
 * @code
 * QStringList l;
 * l << QpamatDebug::DEFAULT_COMPONENT;
 * QpamatDebug::instance()->setFilterComponents(l);
 * @endcode
 *
 * And to redirect the logging to a file:
 *
 * @code
 * QpamatDebug::instance()->redirectFile("bla.out");
 * @endcode
 *
 * To restore the logging to standard error:
 *
 * @code
 * QpamatDebug::instance()->redirectConsole();
 * @endcode
 *
 * In any case, to specify the component, you have to separate the debug message
 * from the component by a single tab. Example:
 *
 * @code
 * qDebug("chipcard\tmessage");
 * @endcode
 *
 * @ingroup misc
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

QpamatDebug *QpamatDebug::m_instance = NULL;

/// Default component. Can be used for example for filtering.
const QString QpamatDebug::DEFAULT_COMPONENT = "default";


/**
 * @brief Returns the only instance of a QpamatDebug class.
 *
 * Since QpamatDebug is a singleton, this is the access function.
 *
 * @return A pointer of the only instance. It cannot return @c NULL.
 */
QpamatDebug *QpamatDebug::instance()
{
    if (!m_instance) {
        m_instance = new QpamatDebug();
    }
    return m_instance;
}


/**
 * @brief Converts a Qt message type to a string
 *
 * It returns:
 *
 *  - @c "DEBUG" for @c QtDebugMsg
 *  - @c "WARNING" for @c QtWarningMsg
 *  - @c "CRITICAL" for @c QtCriticalMsg
 *  - @c "FATAL" for @c QtFatalMsg
 *
 * @return the string as describe above.
 */
QString QpamatDebug::typeToString(QtMsgType type)
{
    switch (type) {
        case QtDebugMsg:        
            return "DEBUG";
        case QtWarningMsg:      
            return "WARNING";
        case QtCriticalMsg:     
            return "CRITICAL";
        case QtFatalMsg:
            return "FATAL";
    }
}


/**
 * @brief Message handler to be used in qInstallMsgHandler
 *
 * See also QpamatDebug::message().
 *
 * @param[in] type the severity of the Qt message
 * @param[in] msg the message
 */
void QpamatDebug::msgHandler(QtMsgType type, const char *msg)
{
    QpamatDebug::instance()->message(type, msg);
}


/**
 * @brief Constructor
 *
 * Creates a new QpamatDebug object. Since this is a singleton, this constructor
 * is private. Always use QpamatDebug::instance() to access the object.
 */
QpamatDebug::QpamatDebug()
    : m_msgHandler(new StderrMsgHandler)
    , m_msgLevel(QtWarningMsg)
{}


/**
 * @brief Destructor
 */
QpamatDebug::~QpamatDebug()
{
    delete m_msgHandler;
}


/**
 * @brief Object method for the message
 *
 * This function does the real work. It filters the message, separates component
 * from the real message and finally calls the message handler. Normally you
 * want to use qDebug(), qWarning(), qCritical() or qFatal(). This Qt function
 * then calls QpamatDebug::msgHandler() which then calls this function.
 *
 * @param[in] type the severity of the Qt message
 * @param[in] msg the message
 */
void QpamatDebug::message(QtMsgType type, const char *msg)
{
    // filter severity
    if (type < m_msgLevel) {
        return;
    }

    // get component and message
    QString messagePart, componentPart;
    QString qmsg(msg);
    if (qmsg.contains("\t")) {
        componentPart = qmsg.section('\t', 0, 0);
        messagePart = qmsg.section('\t', 1);
    } else {
        componentPart = DEFAULT_COMPONENT;
        messagePart = qmsg;
    }

    // filter component
    if (m_components.size() > 0) {
        if (!m_components.contains(componentPart)) {
            return;
        }
    }

    // format date
    QDateTime current(QDateTime::currentDateTime());
    QString date = current.toString("yyyy-MM-dd hh:mm:ss");

    m_msgHandler->output(type, date, componentPart, messagePart);
}

/**
 * @brief Sets the maximum message level.
 *
 * For example, when setMessageLevel(QtCriticalMsg) is called, only messages of
 * severity QtCriticalMsg and QtFatalMsg are printed while QtDebugMsg and
 * QtWarningMsg are suppressed.
 *
 * @param[in] level the new debugging level
 */
void QpamatDebug::setMessageLevel(QtMsgType level)
{
    m_msgLevel = level;
}

/** 
 * @brief Sets the components to filter
 *
 * When no components are set (empty @p components list), then it's not filtered
 * for components. You have to explicitly include QpamatDebug::DEFAULT_COMPONENT
 * if the default component (no component specified when calling qDebug(),
 * qWarning(), qFatal() or qCritical()) should be included.
 *
 * To reset filtering, call with an empty @p components.
 * 
 * @param[in] components the component list
 */
void QpamatDebug::setFilterComponents(const QStringList &components)
{
    m_components = components;
}


/** 
 * @brief Redirects the log messages to stderr
 *
 * This is the default behaviour.
 */
void QpamatDebug::redirectConsole()
{
    delete m_msgHandler;
    m_msgHandler = new StderrMsgHandler;
}



/** 
 * @brief Redirects the log messages to a file
 *
 * If the file cannot be opened, the fallback is console logging.
 * 
 * @param[in] filename the file name, relative to the working directory of the
 *                     application. If @p filename is QString::null, then
 *                     redirectConsole() is called internally.
 */
void QpamatDebug::redirectFile(const QString &filename)
{
    if (filename.isNull()) {
        redirectConsole();
        return;
    }

    m_msgHandler = new FileMsgHandler(filename);
    if (!m_msgHandler->open()) {
        std::cerr << "(debug) Opening file '" << filename.toLocal8Bit().data()
                  << "' failed. Using stderr." << std::endl;
        delete m_msgHandler;
        redirectConsole();
    }
}

// vim: set sw=4 ts=4 et:
