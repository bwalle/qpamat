#!/bin/bash
#
# (c) 2008, Bernhard Walle <bernhard.walle@gmx.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

VERSION=$(grep 'SET.*\<VERSION_STRING\>' CMakeLists.txt \
          |sed 's/.*"\(.*\)".*/\1/g')

hg archive -p qpamat-$VERSION/ -t tbz2 qpamat-$VERSION.tar.bz2
if [ $? -eq 0 ] ; then
    echo "qpamat-$VERSION.tar.bz2 ready." >&2
else
    echo "Program failed." >&2
fi
