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

#include <boost/lambda/lambda.hpp>

// before the include of <sys/mman.h> to get the Q_WS_X11 define
#include <QDebug>

#ifdef Q_WS_X11
#  include <sys/mman.h>
#endif

#include "securestring.h"

/**
 * \class SecureString
 *
 * \brief String impelementation which takes care that the memory is not swapped out
 *
 * This string class takes care that the data is not swapped out to disk but kept in memory.
 * How that is done depends on the operating system. Currently, only mlock() is supported
 * on Unix platforms.
 *
 * This class is Unicode-aware because it uses UTF-8 internally to store the data. If you use
 * QString or a UTF-8 C-String, you can use Unicode. If you use a local 8-bit encoding like
 * ISO-8859-1, you need to convert the data to UTF-8 first manually.
 *
 * @ingroup misc
 * @author Bernhard Walle
 */

/**
 * @brief Checks if the current platform (operating system) supports memory locking.
 *
 * Currently only Linux and a few other Unices supports memory locking.
 *
 * @return \c true if memory locking is supported, \c false otherwise
 */
bool SecureString::platformSupportsLocking()
    throw ()
{
#ifdef _POSIX_MEMLOCK_RANGE
    return true;
#else
    return false;
#endif
}

/**
 * @brief Creates a new SecureString from a C string representation.
 *
 * @param [in] text the C-String representation
 * @throw std::bad_alloc if the memory of the string cannot be allocated
 */
SecureString::SecureString(const char *text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text);
}

/**
 * @brief Creates a new SecureString from a std::string represenation.
 *
 * This additional constructor is only provided for convenience. It's the same as calling
 *
 * @code
 * std::string mystring("bla");
 * SecureString str(bla.c_str());
 * @endcode
 *
 * @param[in] text the std::string represenation
 * @throw std::bad_alloc if the memory of the string cannot be allocated
 */
SecureString::SecureString(const std::string &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.c_str());
}

/**
 * @brief Creates a SecureString from a QString.
 *
 * Internally, we call lock() after allocating the space for the string.
 *
 * @param [in] text the QString representation of the string
 * @throw std::bad_alloc if the memory of the string cannot be allocated
 */
SecureString::SecureString(const QString &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.toUtf8().data());
}

/**
 * @brief Creates a SecureString from another SecureString.
 */
SecureString::SecureString(const SecureString &text)
    throw (std::bad_alloc)
    : m_text(0)
    , m_locked(false)
{
    fromCString(text.utf8());
}

/**
 * @brief Assignment of a SecureString to a SecureString.
 *
 * @param [in] text SecureString the other SecureString that should be assigned
 *                  to the current SecureString
 * @throw std::bad_alloc if the memory of the string cannot be allocated
 */
SecureString &SecureString::operator=(const SecureString& text)
    throw (std::bad_alloc)
{
    if (m_text) {
        unlock();
        smash();
        delete[] m_text;
    }

    fromCString(text.utf8());
    return *this;
}

/**
 * @brief Checks if \p text is less than this string.
 *
 * The check is not locale-aware. However, since we store only passwords in SecureString
 * it's more important that this operation is there to be able to use that SecureString in
 * standard containers and that the results are always the same than that the results are
 * really "correct".
 *
 * The method does not throw.
 *
 * @param [in] text the SecureString to compare with
 * @return \c true if \p text is less than this string, \c false otherwise
 */
bool SecureString::operator<(const SecureString &text) const
    throw ()
{
    return strcmp(utf8(), text.utf8()) < 0;
}

/**
 * @brief Checks if \p text is equal than this string.
 *
 * The method does not throw.
 *
 * @param [in] text the SecureString to compare with
 * @return \c true if \p text is equal than this string, \c false otherwise
 */
bool SecureString::operator==(const SecureString &text) const
    throw ()
{
    return strcmp(utf8(), text.utf8()) == 0;
}

/**
 * @brief Creates a new SecureString from a std::string representation.
 */
SecureString::~SecureString()
    throw ()
{
    unlock();
    smash();
    delete[] m_text;
}

/**
 * @brief Checks if the SecureString is really locked into memory.
 *
 * If the string is not locked into memory, that can have multiple reasons:
 *
 *  - Memory locking is not implemented in SecureString for that operating system.
 *  - Memory locking is implemented, but the current user doesn't have sufficient permission to lock
 *    memory.
 *  - The quota for locking memory for the current user and/or process is full.
 *
 * The method never throws.
 *
 * @return \c true if the string is locked into memory, \c false otherwise
 */
bool SecureString::isLocked() const
    throw ()
{
    return m_locked;
}

/**
 * @brief Returns an UTF-8 representation of the stored text.
 *
 * Please note that this points to an internal pointer of the SecureString. This means *
 * that the memory is valid as long as the SecureString is valid, but not longer. Don't
 * call free(), delete or delete[] on the returned string.
 *
 * @return the UTF-8 representation as zero-terminated string
 */
const char *SecureString::utf8() const
    throw ()
{
    return m_text;
}

/**
 * @brief Returns a QString (without any memory protection!) of the stored text.
 *
 * This is only a convenience method to replace
 *
 * @code
 * QString::fromUtf8(SecureString::utf8())
 * @endcode
 *
 * calls.
 *
 * @return a QString
 */
QString SecureString::qString() const
    throw ()
{
    return QString::fromUtf8(utf8());
}

/**
 * @brief Returns the number of characters that it takes to display that SecureString on
 *        the screen.
 *
 * It's important that this is not the number of bytes in UTF-8 encoding. It's the number
 * of user-visible characters. Of course, when you use UCS-4 (or practially: UCS-2), the
 * number is equal.
 *
 * @return the number of characters
 */
size_t SecureString::length() const
    throw ()
{
    // according to the Unicode FAQ [http://www.cl.cam.ac.uk/~mgk25/unicode.html]
    // we have to count characters not in the range [0x80; 0xBF].
    return std::count_if(
                reinterpret_cast<unsigned char *>(m_text),
                reinterpret_cast<unsigned char *>(m_text) + strlen(m_text),
                boost::lambda::_1 < 0x80 || boost::lambda::_1 > 0xBF);
}

/**
 * @brief Returns the size (in bytes) of the SecureString in UTF-8 encoding.
 *
 * Consider the function SecureString::length() if you need the length that it takes to
 * display the string on the screen.
 *
 * @return the number of bytes
 */
size_t SecureString::size() const
    throw ()
{
    return strlen(m_text);
}

/**
 * @brief Shared implementation for all the constructors.
 *
 * C++ doesn't allow (contrary to Java) calling of another constructor in the constructor.
 * So we use that implementation to share code. <tt>:-)</tt>
 *
 * @param [in] text the text as C-String represenation
 * @throw std::bad_alloc if we couldn't allocate the necessary memory
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

/**
 * @brief Locks the string in m_text into memory.
 *
 * If the string cannot be locked, the implementation does \b not throw any exception.
 * Instead, it issues a warning with qDebug(). If that is not sufficient for you, you can
 * check with SecureString::isLocked() if the memory is really locked.
 *
 * If m_isLocked is \c true which means that the string is already locked, the function
 * silently returns.
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
#endif
}

/**
 * @brief Unlocks the string in m_text from memory.
 *
 * If m_isLocked is \c false which means that the string is not locked, the function
 * silently returns.
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
#endif
}

/**
 * @brief Overwrites the data with garbage.
 *
 * This function should be called before the data is delete[]'d. Currently we overwrite
 * the contents of the data with zeroes, but we could change that to random data. Should
 * not matter because it's not a hard disk but only memory contents.
 */
void SecureString::smash()
    throw ()
{
    std::fill(m_text, m_text+strlen(m_text), '\0');
}

// vim: set sw=4 ts=4 et:
