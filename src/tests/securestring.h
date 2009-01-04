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

class TestSecureString : public QObject {
    Q_OBJECT

    private slots:
        void testCtor() const;
        void testConversion() const;
        void testEqual() const;
        void testAssignment() const;
        void testCopyCtor() const;
        void testLocking() const;
        void testSize() const;
};

// :maxLineLen=100:shiftWidth=4:tabSize=4:noTabs=true:
