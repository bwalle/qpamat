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
#include <cstring>

#include <QObject>
#include <QtTest/QtTest>

#include <util/securestring.h>
#include <tests/securestring.h>

/*!
    \class TestSecureString

    \brief Tests for the SecureString class

    \ingroup unittest
    \author Bernhard Walle
*/


/*!
    Tests the copy constructor.
*/
void TestSecureString::testCtor() const
{
    SecureString a("blubb");
    std::string bStd("blubb");
    SecureString b(bStd);
    QString cQstring("blubb");
    SecureString c(cQstring);

    QVERIFY(a == b);
    QVERIFY(b == c);
    QVERIFY(a == c);
}

/*!
    Tests the return operators.
*/
void TestSecureString::testConversion() const
{
    const char aChar[] = "blubb";
    SecureString a(aChar);
    QString bQString("blubb");
    SecureString b(bQString);

    QVERIFY(b.qString() == bQString);
    QVERIFY(strcmp(a.utf8(), aChar) == 0);
}

/*!
    Tests the equal operator of SecureString.
*/
void TestSecureString::testEqual() const
{
    SecureString a("blaFasel");
    SecureString b("blaFasel");
    SecureString c("blaFaSel");

    QVERIFY(a == b);
    QVERIFY(b == a);
    QVERIFY(a != c);
    QVERIFY(b != c);
}

/*!
    Tests the assignment operator.
*/
void TestSecureString::testAssignment() const
{
    SecureString a("blaFasel");
    a = "blub";

    QVERIFY(a == "blub");
}

/*!
    Tests the copy constructor.
*/
void TestSecureString::testCopyCtor() const
{
    SecureString a("blaFaSel");
    SecureString b(a);

    QVERIFY(a == b);
    if (SecureString::platformSupportsLocking()) {
        QVERIFY(a.isLocked());
        QVERIFY(b.isLocked());
    }
}

/*!
    Checks if the memory is actually locked.
*/
void TestSecureString::testLocking() const
{
    if (!SecureString::platformSupportsLocking())
        return;

    SecureString s("bla");
    QVERIFY(s.isLocked());
}

/*!
    Checks if the size() operator actually returns the number of bytes.
*/
void TestSecureString::testSize() const
{
    const char teststring[5] = { 0x54, 0xc3, 0xa4, 0x73, 0x74 }; // "Täst" in UTF-8
    SecureString s(teststring);

    QVERIFY(s.length() == 4);
    QVERIFY(s.size() == 5);
}

QTEST_MAIN(TestSecureString)

// :maxLineLen=100:shiftWidth=4:tabSize=4:noTabs=true:
