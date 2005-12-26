# Id: $Id$
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
#  Adapt these variables to your system

PREFIX             = /usr/local
SHAREDIR           = $${PREFIX}/share
DOCDIR             = $${SHAREDIR}/doc/packages
BINDIR             = $${PREFIX}/bin

# set this if you installed OpenSSL in a non-standard place
# INCLUDEPATH += /usr/local/ssl/include

# On Windows, this is the default installation path of OpenSSL. Change this
# if needed. Adjust to your compiler if needed.
win32:INCLUDEPATH += c:\openssl\include
win32:LIBS        += -lgdi32 c:\openssl\lib\MinGW\ssleay32.a \
                     c:\openssl\lib\MinGW\libeay32.a

################################################################################

VERSION_STRING     = 0.5.1
MAJOR_VERSION      = 0
MINOR_VERSION      = 5
PATCH_VERSION      = 1

################################################################################

INCLUDEPATH += src/

SOURCES     =                                   \
    src/dialogs/passworddialog.cpp              \
    src/dialogs/newpassworddialog.cpp           \
    src/dialogs/configurationdialog.cpp         \
    src/dialogs/showpassworddialog.cpp          \
    src/dialogs/waitdialog.cpp                  \
    src/dialogs/insertcarddialog.cpp            \
    src/dialogs/cardpinvalidator.cpp            \
    src/dialogs/aboutdialog.cpp                 \
    src/widgets/filelineedit.cpp                \
    src/widgets/fontchoosebox.cpp               \
    src/widgets/copylabel.cpp                   \
    src/widgets/focuslineedit.cpp               \
    src/widgets/listboxlabeledpict.cpp          \
    src/widgets/listboxdialog.cpp               \
    src/widgets/cursor.cpp                      \
    src/security/encodinghelper.cpp             \
    src/security/passwordhash.cpp               \
    src/security/abstractencryptor.cpp          \
    src/security/symmetricencryptor.cpp         \
    src/security/collectencryptor.cpp           \
    src/security/randompasswordgenerator.cpp    \
    src/security/externalpasswordgenerator.cpp  \
    src/security/passwordgeneratorfactory.cpp   \
    src/security/passwordchecker.cpp            \
    src/security/hybridpasswordchecker.cpp      \
    src/security/masterpasswordchecker.cpp      \
    src/smartcard/cardexception.cpp             \
    src/smartcard/memorycard.cpp                \
    src/smartcard/nosuchlibraryexception.cpp    \
    src/smartcard/notinitializedexception.cpp   \
    src/util/stringdisplay.cpp                  \
    src/util/singleapplication.cpp              \
    src/util/docktimeoutapplication.cpp         \
    src/util/windowfunctions.cpp                \
    src/ext/trayicon.cpp                        \
    src/datareadwriter.cpp                      \
    src/timerstatusmessage.cpp                  \
    src/randompassword.cpp                      \
    src/treeentry.cpp                           \
    src/property.cpp                            \
    src/tree.cpp                                \
    src/settings.cpp                            \
    src/qpamat.cpp                              \
    src/help.cpp                                \
    src/rightpanel.cpp                          \
    src/rightlistview.cpp                       \
    src/southpanel.cpp                          \
    src/main.cpp                                

unix:!mac {
    SOURCES += src/ext/trayicon_x11.cpp
}
win32: {
    SOURCES += src/ext/trayicon_win.cpp
}
mac: {
    SOURCES += src/ext/trayicon_mac.cpp
}


# -----------------------------------------------------------------------------

HEADERS     =                                   \
    src/dialogs/passworddialog.h                \
    src/dialogs/newpassworddialog.h             \
    src/dialogs/newpassworddialogprivate.h      \
    src/dialogs/configurationdialog.h           \
    src/dialogs/configurationdialogprivate.h    \
    src/dialogs/showpassworddialog.h            \
    src/dialogs/waitdialog.h                    \
    src/dialogs/insertcarddialog.h              \
    src/dialogs/cardpinvalidator.h              \
    src/dialogs/aboutdialog.h                   \
    src/widgets/filelineedit.h                  \
    src/widgets/fontchoosebox.h                 \
    src/widgets/copylabel.h                     \
    src/widgets/focuslineedit.h                 \
    src/widgets/listboxlabeledpict.h            \
    src/widgets/listboxdialog.h                 \
    src/widgets/cursor.h                        \
    src/global.h                                \
    src/security/encodinghelper.h               \
    src/security/passwordhash.h                 \
    src/security/encryptor.h                    \
    src/security/abstractencryptor.h            \
    src/security/symmetricencryptor.h           \
    src/security/collectencryptor.h             \
    src/security/passwordgenerator.h            \
    src/security/randompasswordgenerator.h      \
    src/security/externalpasswordgenerator.h    \
    src/security/passwordgeneratorfactory.h     \
    src/security/passwordchecker.h              \
    src/security/hybridpasswordchecker.h        \
    src/security/masterpasswordchecker.h        \
    src/randompassword.h                        \
    src/smartcard/cardexception.h               \
    src/smartcard/memorycard.h                  \
    src/smartcard/nosuchlibraryexception.h      \
    src/smartcard/notinitializedexception.h     \
    src/util/stdrandomnumbergenerator.h         \
    src/util/stringdisplay.h                    \
    src/util/singleapplication.h                \
    src/util/docktimeoutapplication.h           \
    src/util/windowfunctions.h                  \
    src/ext/trayicon.h                          \
    src/datareadwriter.h                        \
    src/timerstatusmessage.h                    \
    src/treeentry.h                             \
    src/property.h                              \
    src/settings.h                              \
    src/qpamat.h                                \
    src/tree.h                                  \
    src/rightpanel.h                            \
    src/rightlistview.h                         \
    src/southpanel.h                            \
    src/help.h

# -----------------------------------------------------------------------------

TRANSLATIONS =                                  \
    ts/de.ts

# -----------------------------------------------------------------------------

UI_DIR = out
RESOURCES     = qpamat.qrc
win32:RC_FILE = share/win32/qpamat_win32.rc


# -----------------------------------------------------------------------------

QT           += xml qt3support 

isEmpty(static) {
  CONFIG     += warn_on qt exceptions thread
} else {
  LIBS       += /usr/lib/qt3/lib/libqt-mt.a -lfreetype -lXft -lz -lpthread \
                 -lpng -lX11 -lXext -lXinerama -lXi -lSM -lXcursor -lXrandr -lGL
  CONFIG     += qt warn_on exceptions thread
}

# -----------------------------------------------------------------------------

DEFINES    += VERSION_STRING=\"$$VERSION_STRING\" 
DEFINES    += MAJOR_VERSION=$$MAJOR_VERSION
DEFINES    += MINOR_VERSION=$$MINOR_VERSION
DEFINES    += PATCH_VERSION=$$PATCH_VERSION

isEmpty (debug) {
  #DEFINES  += QT_NO_CHECK
  CONFIG    += release
} else {
  CONFIG   += debug
  DEFINES  += DEBUG
  DEFINES  += TRACE
}

# -----------------------------------------------------------------------------

unix:LIBS  += -lssl -lm -lcrypto

# -----------------------------------------------------------------------------

MOC_DIR     = out
OBJECTS_DIR = out
UI_DIR      = out
RCC_DIR     = out
DESTDIR     = bin

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = documentation
documentation.commands      = doxygen  ;
documentation.commands     += cd doc/user/en && rm -rf manual && db2html manual.sgml && cd - 
QMAKE_EXTRA_UNIX_TARGETS   += documentation

#
# make a tarball on Unix with "make release"
maketarball.target          = tarball
maketarball.commands        = svn export `pwd` /tmp/qpamat-$$VERSION_STRING &&
maketarball.commands       += pushd . &&
maketarball.commands       += cd /tmp/qpamat-$$VERSION_STRING &&
maketarball.commands       += qmake qpamat.pro &&
maketarball.commands       += make documentation &&
maketarball.commands       += rm Makefile &&
maketarball.commands       += cd /tmp &&
maketarball.commands       += tar cvfj qpamat-$${VERSION_STRING}.tar.bz2 qpamat-$$VERSION_STRING &&
maketarball.commands       += popd &&
maketarball.commands       += mv /tmp/qpamat-$${VERSION_STRING}.tar.bz2 . &&
maketarball.commands       += rm -r /tmp/qpamat-$$VERSION_STRING
QMAKE_EXTRA_UNIX_TARGETS   += maketarball

#
# Tags
tags.target                 = tagfile
tags.commands               = ctags -R src/ &&
tags.commands              += echo "Tag created"
QMAKE_EXTRA_UNIX_TARGETS   += tags

# ------------------------------------------------------------------------------

#
# Installation
i_documentation.path        = $$DOCDIR
i_documentation.files      += doc/qpamat.dtd README COPYING 
INSTALLS                   += i_documentation

i_dictionary.path           = $$SHAREDIR/qpamat
i_dictionary.files         += share/dicts
INSTALLS                   += i_dictionary

i_share_doc.path            = $$SHAREDIR/qpamat/doc
i_share_doc.files          += doc/user/en
INSTALLS                   += i_share_doc

i_share.path                = $$SHAREDIR/qpamat
i_share.files              += COPYING
INSTALLS                   += i_share

i_messages.path             = $$SHAREDIR/qpamat/translations
i_messages.files           += ts/*.qm
INSTALLS                   += i_messages

i_icons.path                = $$SHAREDIR/pixmaps
i_icons.files              += images/qpamat_*.png
INSTALLS                   += i_icons

i_binary.path               = $$BINDIR
i_binary.files             += bin/qpamat*
INSTALLS                   += i_binary

# vim: ft=text et

