#!/bin/sh

cd ..
VERSION=`grep ^VERSION_STRING qpamat.pro | sed -e 's/.*= //g'`

mkdir qpamat
mkdir qpamat/bin
mkdir -p qpamat/share/qpamat/dicts
mkdir -p qpamat/share/qpamat/translations
mkdir -p qpamat/share/qpamat/doc

cp qpamat.exe qpamat/bin
cp $QTDIR/lib/*.dll qpamat/bin
cp COPYING qpamat/share/qpamat
cp -r doc/user/en qpamat/share/qpamat/doc
cp share/dicts/*.txt qpamat/share/qpamat/dicts
find qpamat -name .svn -exec rm -fr {} \;

zip -r qpamat-$VERSION qpamat
rm -fr qpamat
