#!/bin/sh

cd ..
mkdir qpamat
mkdir qpamat/bin
mkdir -p qpamat/share/qpamat/dicts
mkdir -p qpamat/share/qpamat/translations
mkdir -p qpamat/share/qpamat/doc

cp qpamat.exe qpamat/bin
cp COPYING qpamat/share/qpamat
cp -r doc/user/en qpamat/share/qpamat/doc
cp share/dicts/*.txt qpamat/share/qpamat/dicts
find qpamat -name .svn -exec rm -fr {} \;

zip -r qpamat qpamat
rm -fr qpamat
