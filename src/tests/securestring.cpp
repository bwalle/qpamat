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
    Tests the equal operator of SecureString.
*/
void TestSecureString::testEqual()
{
    SecureString a("blaFasel");
    SecureString b("blaFasel");
    SecureString c("blaFaSel");

    QVERIFY(a == b);
    QVERIFY(b == a);
    QVERIFY(a != c);
    QVERIFY(b != c);
}

QTEST_MAIN(TestSecureString)

// :maxLineLen=100:shiftWidth=4:tabSize=4:noTabs=true:
