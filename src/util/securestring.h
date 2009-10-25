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
#ifndef SECURESTRING_H
#define SECURESTRING_H

#include <stdexcept>
#include <string>

#include <QString>

class SecureString
{
    public:
        SecureString()
        throw ();

        SecureString(const char *text)
        throw (std::bad_alloc);

        SecureString(const std::string &text)
        throw (std::bad_alloc);

        SecureString(const QString &text)
        throw (std::bad_alloc);

        SecureString(const SecureString &text)
        throw (std::bad_alloc);

        virtual ~SecureString()
        throw ();

        SecureString &operator=(const SecureString &text)
        throw (std::bad_alloc);

        bool operator<(const SecureString &text) const
        throw ();

        bool operator<=(const SecureString &text) const
        throw ();

        bool operator>(const SecureString &text) const
        throw ();

        bool operator>=(const SecureString &text) const
        throw ();

        bool operator==(const SecureString &text) const
        throw ();

        bool operator!=(const SecureString &text) const
        throw ();

    public:
        static bool platformSupportsLocking()
        throw ();

    public:
        bool isLocked() const
        throw ();

        const char *utf8() const
        throw ();

        QString qString() const
        throw ();

        size_t length() const
        throw ();

        size_t size() const
        throw ();

    protected:
        void fromCString(const char *text)
        throw (std::bad_alloc);

        void lock()
        throw ();

        void unlock()
        throw ();

        void smash()
        throw ();

    private:
        char *m_text;
        bool m_locked;
        static bool s_warned;
};

#endif // SECURESTRING_H

// vim: set sw=4 ts=4 et:
