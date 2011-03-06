#!/bin/sh
set -e

NAME=qpamat

read VERSION < VERSION
ARCHIVE=$NAME-$VERSION.tar.bz2
git archive --format=tar --prefix=$NAME-$VERSION/ master | bzip2 > $ARCHIVE
echo "Archive '$ARCHIVE' ready!"
