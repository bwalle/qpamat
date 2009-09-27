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
#ifndef HYBRIDPASSWORDCHECKER_H
#define HYBRIDPASSWORDCHECKER_H

#include <stdexcept>

#include <QString>
#include <Q3ValueVector>
#include <QStringList>
#include <Q3ValueVector>
#include <QFile>
#include <QMap>

#include "global.h"
#include "passwordchecker.h"

class HybridPasswordChecker : public PasswordChecker
{
    public:
        HybridPasswordChecker(const QString& dictFileName)
            throw (PasswordCheckException);

        double passwordQuality(const QString& password) throw ();

    private:
        QString findLongestWord(const QString& password) const;
        int getNumberOfWordsWithSameOrShorterLength(const QString& password) const;
        int findNumerOfCharsInClass(const QString& chars) const;

    private:
        static StringVector         m_words;
        static QFile::Offset        m_fileSize;
        static QString              m_fileName;
        static QMap<int, int>       m_lengthBeginMap;
};

bool string_length_less(const QString& a, const QString& b);
bool string_length_greater(const QString& a, const QString& b);

#endif // HYBRIDPASSWORDCHECKER_H

// vim: set sw=4 ts=4 et:
