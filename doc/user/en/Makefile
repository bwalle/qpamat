#
# Makefile for QPaMaT-Documentation  
# (c) B. Walle
#

#
# The stylesheet is necessary for A4 paper format
# (we're in Europe, not in U.S.A.)

doc = manual.sgml

db2html = /usr/bin/db2html
sgmltools = /usr/bin/sgmltools
db2html = /usr/bin/db2html
db2pdf = /usr/bin/db2pdf
db2ps = /usr/bin/db2ps
db2dvi = /usr/bin/db2dvi
db2html = /usr/bin/db2html
db2rtf = /usr/bin/db2rtf
stylesheet = ../stylesheet.dsl
nsgmls = /usr/bin/nsgmls
catalog = /usr/share/sgml/CATALOG.docbook_4
gzip = /usr/bin/gzip
mv = /bin/mv

prefix = /usr
sharedir = $(prefix)/share
sysconfdir = /etc
tempdir = /tmp
bindir = $(prefix)/bin
libdir = $(prefix)/lib
docdir = $(prefix)/share/doc/packages
docdirname = html
INSTALL = install -m 0644
INSTALLDIR = install -d


#
# we will not make 'all' but only the 'standard' by default
default: pdf html deltemp
all: ps dvi pdf html rtf deltemp
ps: manual.ps
pdf: manual.pdf
dvi: manual.dvi
html: manual/*
txt: manual.txt
rtf: manual.rtf

manual.pdf: manual.sgml
	$(db2pdf) -s $(stylesheet) $(doc)

manual.ps: manual.sgml
	$(db2ps) -s $(stylesheet) $(doc)

manual.dvi: manual.sgml
	$(db2dvi) -s $(stylesheet) $(doc)

manual.rtf: manual.sgml
	$(db2rtf) -s $(stylesheet) $(doc)

manual/*: manual.sgml
	$(db2html) -s $(stylesheet) $(doc)

validate:
	$(nsgmls) -s -c $(catalog) $(doc)

clean:
	rm -rf *.dvi *.rtf *.pdf *.ps *.txt *.gz manual/ *.out *.log *.aux *.tex 

deltemp:
	rm -rf *.out *.log *.aux *.tex
