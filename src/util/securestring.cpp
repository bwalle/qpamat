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
#include <stdexcept>
#include <string>
#include <cstring>
#include <cerrno>
#include <algorithm>

// before the include of <sys/mman.h> to get the Q_WS_X11 define
#include <QDebug>

#ifdef Q_WS_X11
#  include <sys/mman.h>
#endif

#include "securestring.h"


/*!
    \class SecureString

    \brief String impelementation which takes care that the memory is not swapped out

    This string class takes care that the data is not swapped out to disk but kept in memory.
    How that is done depends on the operating system. Currently, only mlock() is supported
    on Unix platforms.

    This class is Unicode-aware because it uses UTF-8 internally to store the data. If you use
    QString or a UTF-8 C-String, you can use Unicode. If you use a local 8-bit encoding like
    ISO-8859-1, you need to convert the data to UTF-8 first manually.

    \ingroup misc
    \author Bernhard Walle
*/

/*!
    Creates a new SecureString from a C string representation.

    \param text the C-String representation
    \throw std::bad_alloc if the memory of the string cannot be allocated
*/
SecureString::SecureString(const char *text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text);
}

/*!
    Creates a new SecureString from a std::string represenation. This additional constructor is
    only provided for convenience. It's the same as calling

    \code
std::string mystring("bla");
SecureString str(bla.c_str());
    \endcode

    \param text the std::string represenation
    \throw std::bad_alloc if the memory of the string cannot be allocated
*/
SecureString::SecureString(const std::string &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.c_str());
}

/*!
    Creates a SecureString from a QString.

    Internally, we call lock() after allocating the space for the string.

    \param text the QString representation of the string
    \throw std::bad_alloc if the memory of the string cannot be allocated
*/
SecureString::SecureString(const QString &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.toUtf8().data());
}

/*!
    Creates a SecureString from another SecureString.
*/
SecureString::SecureString(const SecureString &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.utf8());
}

/*!
    Assignment of a SecureString to a SecureString.

    \param text SecureString the other SecureString that should be assigned to the current SecureString
    \throw std::bad_alloc if the memory of the string cannot be allocated
*/
SecureString &SecureString::operator=(const SecureString& text)
    throw (std::bad_alloc)
{
    if (m_text) {
        unlock();
        delete[] m_text;
    }

    fromCString(text.utf8());
    return *this;
}

/*!
    Checks if \p text is less than this string.

    The check is not locale-aware. However, since we store only passwords in SecureString it's more
    important that this operation is there to be able to use that SecureString in standard containers
    and that the results are always the same than that the results are really "correct".

    The method does not throw.

    \param text the SecureString to compare with
    \return \c true if \p text is less than this string, \c false otherwise
*/
bool SecureString::operator<(const SecureString &text)
    throw ()
{
    return strcmp(utf8(), text.utf8()) < 0;
}

/*!
    Checks if \p text is equal than this string.

    The method does not throw.

    \param text the SecureString to compare with
    \return \c true if \p text is equal than this string, \c false otherwise
*/
bool SecureString::operator==(const SecureString &text)
    throw ()
{
    return strcmp(utf8(), text.utf8()) == 0;
}

/*!
    Creates a new SecureString from a std::string representation. This is only
*/
SecureString::~SecureString()
    throw ()
{
    unlock();
    delete[] m_text;
}

/*!
    Returns an UTF-8 representation of the stored text. Please note that this points to an internal
    pointer of the SecureString. This means that the memory is valid as long as the SecureString is
    valid, but not longer. Don't call free(), delete or delete[] on the returned string.

    \return the UTF-8 representation as zero-terminated string
*/
const char *SecureString::utf8() const
    throw ()
{
    return m_text;
}

/*!
    Returns a QString (without any memory protection!) of the stored text.

    This is only a convenience method to replace
    \code
QString::fromUtf8(SecureString::utf8())
    \endcode

    calls.

    \return a QString
*/
QString SecureString::qString() const
    throw ()
{
    return QString::fromUtf8(utf8());
}

/*!
    Shared implementation for all the constructors. C++ doesn't allow (contrary to Java)
    calling of another constructor in the constructor. So we use that implementation to share
    code. <tt>:-)</tt>

    \param text the text as C-String represenation
    \throw std::bad_alloc if we couldn't allocate the necessary memory
*/
void SecureString::fromCString(const char *text)
    throw (std::bad_alloc)
{
    // don't use strdup() to be able to allocate with new[] instead of malloc()
    size_t len = strlen(text);
    m_text = new char[len+1];

    std::copy(text, text+len+1, m_text);
    lock();
}

/*!
    Locks the string in m_text into memory.

    If the string cannot be locked, the implementation does \b not throw any exception. Instead,
    it issues a warning with qDebug(). If that is not sufficient for you, you can check with
    SecureString::isLocked() if the memory is really locked.

    If m_isLocked is \c true which means that the string is already locked, the function silently
    returns.
*/
void SecureString::lock()
    throw ()
{
    // do nothing if the string is already locked
    if (m_locked)
        return;

#ifdef _POSIX_MEMLOCK_RANGE
    // lock the whole string including the trailing newline
    int ret = mlock(m_text, strlen(m_text)+1);
    if (ret != 0)
        qDebug() << "Cannot lock memory " << strerror(errno);
    else
        m_locked = true;
#else
    qDebug() << "Cannot use mlock() on that platform";
#endif
}

/*!
    Unlocks the string in m_text from memory.

    If m_isLocked is \c false which means that the string is not locked, the function silently
    returns.
*/
void SecureString::unlock()
    throw ()
{
    // do nothing if the string is not locked
    if (!m_locked)
        return;

#ifdef _POSIX_MEMLOCK_RANGE
    int ret = munlock(m_text, strlen(m_text)+1);
    if (ret != 0)
        qDebug() << "Cannot unlock memory " << strerror(errno);
#else
    qDebug() << "Cannot use munlock() on that platform";
#endif
}

// :maxLineLen=100:shiftWidth=4:tabSize=4:noTabs=true:
