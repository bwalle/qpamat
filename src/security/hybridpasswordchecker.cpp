/*
 * Id: $Id: hybridpasswordchecker.cpp,v 1.2 2003/12/29 19:57:18 bwalle Exp $
 * -------------------------------------------------------------------------------------------------
 * 
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
#include <algorithm>
#include <cmath>

#include <qstring.h>
#include <qvaluevector.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdatetime.h>

#include "global.h"
#include "hybridpasswordchecker.h"


// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

StringVector    HybridPasswordChecker::m_words;
QFile::Offset   HybridPasswordChecker::m_fileSize = 0;
QString         HybridPasswordChecker::m_fileName;
QMap<int, int>  HybridPasswordChecker::m_lengthBeginMap;
QDateTime       HybridPasswordChecker::m_lastModifed;



/*!
    \class HybridPasswordChecker
    
    \brief Hybrid password checker.
    
    This password checker works according to the algorithm described at 
    https://passwortcheck.datenschutz.ch (only in German).
    
    The dictionary file consists of single word in each line. It needs to be \b sorted according 
    to the length of the words. The first word must be the longest word and the last word must
    be the shortest word.
    
    \ingroup security
    \author Bernhard Walle
    \version $Revision: 1.2 $
    \date $Date: 2003/12/29 19:57:18 $
*/


/*!
    Creates a new instance of a HybridPasswordChecker. Caching is performed, i.e. only
    the first creation of the object reads the file. Following instantiations check the
    modification time (see \p checkModtime) and if it is still the same, the cached data 
    is used. This increases performance dramatically but needs more memory. Since modern
    computers have much memory this is better than having a slow program.
    \param dictFileName the name of the dictionary.
    \param checkModtime if checkModtime is \c true the constructor checks the modification
                        time of the file and re-reads the dictionary if necessary. On slow
                        filesystems (NFS), this can be set to \c false to increase
                        performance.
    \exception PasswordCheckException if the file does not exist or if the file cannot be opened
*/
HybridPasswordChecker::HybridPasswordChecker(const QString& dictFileName, bool checkModtime)
            throw (PasswordCheckException)
{
    if (!QFile::exists(dictFileName))
    {
        throw PasswordCheckException( QString("The file %1 does not exist.").arg(
            dictFileName).latin1());
    }
    
    // do we need to re-read
    if (m_words.isEmpty() || (m_fileName != dictFileName) || 
             (checkModtime && (QFileInfo(dictFileName).lastModified() != m_lastModifed)) )
    {
        
        qDebug("!!!! Re-reading the file !!!!!");
        
        QFile file(dictFileName);
        if (!file.open(IO_ReadOnly))
        {
            throw PasswordCheckException( QString("Could not open the file %1.").arg(
                dictFileName).latin1() );
        }
        
        // read in memory
        QByteArray bytes = file.readAll();
        file.close();
        m_lastModifed = QFileInfo(file).lastModified();
        m_fileName = dictFileName;
        
        // check the number of lines to increase speed
        uint numberOfLines = 0;
        for (uint i = 0; i < bytes.size(); ++i)
        {
            if (bytes[i] == '\n')
            {
                ++numberOfLines;
            }
        }
        
        m_words.reserve(numberOfLines+5);
        QTextStream fileStream(bytes, IO_ReadOnly);
        int oldLength = 0;
        int length = 0;
        int index = 0;
        while (QString text = fileStream.readLine())
        {
            length = text.length();
            if (length != oldLength)
            {
                if (length == 1)
                {
                    break;
                }
                m_lengthBeginMap.insert(length, index);
                oldLength = length;
            }
            m_words.append(text);
            ++index;
        }
    }
}


/*!
    Checks the password.
    \param password the password to check
    \return the number of days that a cracker needs to crack according to the password 
            checker
*/
double HybridPasswordChecker::passwordQuality(const QString& password) throw ()
{
    const QString longest = findLongestWord(password);
    int pos = password.find(longest, 0, false);
    QString rest = password;
    rest.remove(pos, longest.length());
    uint Z = findNumerOfCharsInClass(rest);
    uint L = rest.length();
    uint W = getNumberOfWordsWithSameOrShorterLength(longest);
    if (W == 0)
    {
        W = 1;
    }
    uint P = longest ? (password.length() - longest.length() + 1 ) : password.length();
    
    qDebug("-----------------------------------------------------");
    qDebug("Password = %s", password.latin1());
    qDebug("Z = %d, L = %d, W = %d, P = %d", Z, L, W, P);
    return double( (powl( double(Z), double(L)) * W * P) / CRACKS_PER_SECOND / 86400);
}


/*!
    Finds the longest word that occures in \p password and is in the dictionary.
    \param password the password
    \return the longest word
*/
QString HybridPasswordChecker::findLongestWord(const QString& password) const
{
    int initialPosition = 0;
    int pwLength = password.length();
    while (!m_lengthBeginMap.contains(pwLength) && pwLength >= 0)
    {
        --pwLength;
    }
    if (pwLength < 0)
    {
        return "";
    }
    initialPosition = m_lengthBeginMap[pwLength];
    
    for (StringVector::iterator it = m_words.begin() + initialPosition; it != m_words.end(); ++it)
    {
        if (password.contains(*it, false))
        {
            return *it;
        }
    }
    return "";
}


/*!
    Returns the number of words that have the same length or a shorther length as the
    given \p word.
    \param word the word
    \return the number of words
*/
int HybridPasswordChecker::getNumberOfWordsWithSameOrShorterLength(const QString& word) const
{
    int len = word.length();
    while (!m_lengthBeginMap.contains(len) && len >= 0)
    {
        --len;
    }
    if (len < 0)
    {
        return 0;
    }
    return m_lengthBeginMap[len];
}


/*!
    Finds the number of characters in the character class of the word. For example if \p chars
    is \c abcd, then all characters are lowercase letters so we must only search in the
    character class "lowercase letters" and so the function return 26.
    \param chars the word
    \return the number in that character class
    \todo Verify the documentation
*/
int HybridPasswordChecker::findNumerOfCharsInClass(const QString& chars) const
{
    int ret = 0;
    bool hasDigits = false, hasLowercase = false, hasUppercase = false, hasUmlauts = false;
    bool hasSpecial = false, hasOther = false;
    for (uint i = 0; i < chars.length(); ++i)
    {
        QChar c = chars[i];
        if (c.latin1() == 0)
        {
            return 255;
        }
        else if (!hasDigits || c >= '0' && c <= '9' || c == ' ')
        {
            hasDigits = true;
        }
        else if (!hasLowercase || c >= 'a' && c <= 'z')
        {
            hasLowercase = true;
        }
        else if (!hasUppercase || c >= 'A' && c <= 'Z')
        {
            hasUppercase = true;
        }
        else if (!hasSpecial || c == ',' || c == '.' || c == '-' || c == ';' || c == ':' || c == '_' 
            || c == '='
            || c == '(' || c == ')' || c == '*' || c == '+' || c == '?' || c == '"' || c == '$'
            || c == '£' || c == '§' || c == '@' ||c == '#' || c == '%' || c == '&' || c == '/'
            || c == '\\' || c == '{' || c == '}' || c == '[' || c == ']' || c == '!' || c == '^'
            || c == '?' || c == '\'' || c == '?' || c == '`' || c == '~')
        {
            hasSpecial = true;
        }
        else if (!hasUmlauts || c == 'ö' || c == 'ä' || c == 'ü' || c == 'é' || c == 'à' || 
            c == 'è' || c == 'Ö' || c == 'Ä' || c == 'Ü' || c == 'É' || c == 'À' || c == 'È' || 
            c == 'ç')
        {
            hasUmlauts = true;
        }
        else
        {
            hasOther = true;
        }
    }
    
    if (hasDigits)
    {
        ret += 11;
    }
    if (hasLowercase)
    {
        ret += 26;
    }
    if (hasUppercase)
    {
        ret += 26;
    }
    if (hasUmlauts)
    {
        ret += 13;
    }
    if (hasSpecial)
    {
        ret += 30;
    }
    if (hasOther)
    {
        ret += 118;
    }
    qDebug("Ret is %d", ret);
    return ret;
}

// -------------------------------------------------------------------------------------------------

/*!
    \relates HybridPasswordChecker
    Compares the length of two strings.
    \param a the first string
    \param b the second string
    \return \c true if length(a) < length(b)
*/
bool string_length_less(const QString& a, const QString& b)
{
    return a.length() < b.length();
}

/*!
    \relates HybridPasswordChecker
    Compares the length of two strings.
    \param a the first string
    \param b the second string
    \return \c true if length(a) > length(b)
*/
bool string_length_greater(const QString& a, const QString& b)
{
    return a.length() > b.length();
}
