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
#include <QDebug>

/**
 * @file debug.h
 * @ingroup misc
 *
 * @brief Debugging for Qpamat
 *
 * This file defines some macros that makes printing debugging messages more
 * convenient.
 */

/**
 * @brief Default component
 *
 * This is the default component for debugging messages which should be used
 * whenever no real component can be used. This must be a macro since QDebug()
 * object stream print string variable with quotes (<tt>"default"</tt> instead
 * of <tt>default</tt> in that case).
 */
#define DEFAULT_COMPONENT "default"

/**
 * @brief Prints a debugging message
 *
 * Prints a debugging message like qDebug(), but include line number and
 * component information. This is the "modern" usage of qDebug(), returning a
 * QDebug object instead of taking a printf-like string.
 *
 * Example:
 *
 * @code
 * int bla = 30;
 * qpDebug("Security") << "Bla is" << bla;
 * @endcode
 *
 * @param[in] component the component name (DEFAULT_COMPONENT may be used)
 * @return a QDebug object
 */
#define qpDebug(component) \
    (qDebug() << Q_FUNC_INFO << __LINE__ << "\t" << (component) << "\t")

/**
 * @brief Prints a warning message
 *
 * Prints a warning message like qWarning(), but include line number and
 * component information. This is the "modern" usage of qWarning(), returning a
 * QDebug object instead of taking a printf-like string.
 *
 * Example:
 *
 * @code
 * qpWarning("Security") << "Bla is too large!";
 * @endcode
 *
 * @param[in] component the component name (DEFAULT_COMPONENT may be used)
 * @return a QDebug object
 */
#define qpWarning(component) \
    (qWarning() << Q_FUNC_INFO << __LINE__ << "\t" << (component) << "\t")

/**
 * @brief Prints a critical message
 *
 * Prints a warning message like qCritical(), but include line number and
 * component information. This is the "modern" usage of qCritical(), returning a
 * QDebug object instead of taking a printf-like string.
 *
 * Example:
 *
 * @code
 * qpCritical("Security") << "Insecure";
 * @endcode
 *
 * @param[in] component the component name (DEFAULT_COMPONENT may be used)
 * @return a QDebug object
 */
#define qpCritical(component) \
    (qCritical() << Q_FUNC_INFO << __LINE__ << "\t" << (component) << "\t")

/**
 * @brief Prints a fatal message
 *
 * Prints a warning message like qFatal(), but include line number and
 * component information. This is the "modern" usage of qFatal(), returning a
 * QDebug object instead of taking a printf-like string.
 *
 * Example:
 *
 * @code
 * qpFatal("Security") << "We are lost. Terminating.";
 * @endcode
 *
 * @param[in] component the component name (DEFAULT_COMPONENT may be used)
 * @return a QDebug object
 */
#define qpFatal(component) \
    (qFatal() << Q_FUNC_INFO << __LINE__ << "\t" << (component) << "\t")

class MsgHandler;

class QpamatDebug
{
    public:
        static QpamatDebug *instance();
        static void msgHandler(QtMsgType type, const char *msg);
        static QString typeToString(QtMsgType type);

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

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
