# Id: $Id: qpamat.pro,v 1.26 2004/01/03 23:41:37 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
################################################################################


VERSION     = 0.1


INCLUDEPATH += src/
# set this if you installed OpenSSL in a non-standard place
# INCLUDEPATH += /usr/local/ssl/include

SOURCES     =                                   \
    src/dialogs/passworddialog.cpp              \
    src/dialogs/newpassworddialog.cpp           \
    src/dialogs/configurationdialog.cpp         \
    src/dialogs/showpassworddialog.cpp          \
	src/dialogs/waitdialog.cpp					\
    src/widgets/filelineedit.cpp                \
    src/widgets/fontchoosebox.cpp               \
    src/widgets/copylabel.cpp                   \
    src/widgets/focuslineedit.cpp               \
    src/widgets/listboxlabeledpict.cpp          \
    src/security/encodinghelper.cpp             \
    src/security/passwordhash.cpp               \
    src/security/abstractencryptor.cpp          \
    src/security/symmetricencryptor.cpp         \
    src/security/notencryptor.cpp               \
    src/security/collectencryptor.cpp           \
    src/security/randompasswordgenerator.cpp    \
    src/security/externalpasswordgenerator.cpp  \
    src/security/passwordgeneratorfactory.cpp   \
    src/security/passwordchecker.cpp            \
    src/security/hybridpasswordchecker.cpp      \
    src/security/masterpasswordchecker.cpp      \
    src/smartcard/cardexception.cpp             \
    src/smartcard/memorycard.cpp                \
	src/smartcard/nosuchlibraryexception.cpp	\
	src/smartcard/notinitializedexception.cpp	\
    src/util/stringdisplay.cpp                  \
    src/util/singleapplication.cpp              \
    src/timerstatusmessage.cpp            	 	\
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

# -----------------------------------------------------------------------------

HEADERS     =                                   \
    src/dialogs/passworddialog.h                \
    src/dialogs/newpassworddialog.h             \
    src/dialogs/newpassworddialogprivate.h      \
    src/dialogs/configurationdialog.h           \
    src/dialogs/configurationdialogprivate.h    \
    src/dialogs/showpassworddialog.h            \
	src/dialogs/waitdialog.h  					\
    src/widgets/filelineedit.h                  \
    src/widgets/fontchoosebox.h                 \
    src/widgets/copylabel.h                     \
    src/widgets/focuslineedit.h                 \
    src/widgets/listboxlabeledpict.h            \
    src/global.h                                \
    src/security/encodinghelper.h               \
    src/security/passwordhash.h                 \
    src/security/encryptor.h                    \
    src/security/notencryptor.h                 \
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
	src/smartcard/nosuchlibraryexception.h		\
	src/smartcard/notinitializedexception.h		\
    src/util/stringdisplay.h                    \
    src/util/singleapplication.h                \
    src/timerstatusmessage.h               		\
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

isEmpty(static) {
  CONFIG     += warn_on qt exceptions
} else {
  LIBS        += /usr/lib/qt3/lib/libqt-mt.a -lfreetype -lXft -lz -lpthread -lpng -lX11 -lXext -lXinerama -lXi -lSM -lXcursor -lXrandr -lGL
  CONFIG      += x11 warn_on exceptions
}

# -----------------------------------------------------------------------------

DEFINES    += VERSION=\"$$VERSION\"

isEmpty (debug) {
  #DEFINES  += QT_NO_CHECK
  CONFIG    += release
} else {
  CONFIG   += debug
  DEFINES  += DEBUG
  DEFINES  += TRACE
}

QMAKE_CXXFLAGS_WARN_ON 	+= -Wno-unused-parameter

# -----------------------------------------------------------------------------

LIBS       += -lssl -lm -lcrypto

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = doc
documentation.commands      = doxygen qpamat.doxy
QMAKE_EXTRA_UNIX_TARGETS   += documentation

#
# make a tarball on Unix with "make release"
maketarball.target 		    = tarball
maketarball.commands        = cd .. &&
maketarball.commands       += cp -r qpamat qpamat-$$VERSION &&
maketarball.commands       += tar cvfz qpamat-$${VERSION}.tar.gz qpamat-$$VERSION &&
maketarball.commands       += cd - &&
maketarball.commands       += mv ../qpamat-$${VERSION}.tar.gz . &&
maketarball.commands       += rm -r ../qpamat-$$VERSION
QMAKE_EXTRA_UNIX_TARGETS   += maketarball

#
# Tags
tags.target                 = tagfile
tags.commands               = ctags -R src/ &&
tags.commands              += echo "Tag created"
QMAKE_EXTRA_UNIX_TARGETS   += tags

# vim: ft=text et
