# Id: $Id: qpamat.pro,v 1.21 2003/12/18 21:58:40 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
################################################################################

VERSION     = 0.1

# set this if you installed OpenSSL in a non-standard place
# INCLUDEPATH += /usr/local/ssl/include

SOURCES     =                                   \
    src/dialogs/passworddialog.cpp              \
    src/dialogs/newpassworddialog.cpp           \
    src/dialogs/configurationdialog.cpp         \
    src/dialogs/randompassworddialog.cpp        \
    src/widgets/filelineedit.cpp                \
    src/widgets/fontchoosebox.cpp               \
    src/widgets/copylabel.cpp                   \
    src/widgets/focuslineedit.cpp               \
    src/security/encodinghelper.cpp             \
    src/security/passwordhash.cpp               \
    src/security/abstractencryptor.cpp          \
    src/security/symmetricencryptor.cpp         \
    src/security/notencryptor.cpp               \
    src/security/collectencryptor.cpp           \
    src/security/randompasswordgenerator.cpp    \
    src/security/externalpasswordgenerator.cpp  \
    src/security/passwordgeneratorfactory.cpp   \
    src/security/masterpasswordchecker.cpp      \
    src/security/configpasswordchecker.cpp      \
    src/smartcard/cardexception.cpp             \
    src/smartcard/memorycard.cpp                \
    src/util/timerstatusmessage.cpp             \
    src/randompassword.cpp                      \
    src/singleapplication.cpp                   \
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
    src/dialogs/configurationdialog.h           \
    src/dialogs/randompassworddialog.h          \
    src/widgets/filelineedit.h                  \
    src/widgets/fontchoosebox.h                 \
    src/widgets/copylabel.h                     \
    src/widgets/focuslineedit.h                 \
    src/global.h                                \
    src/security/passwordcheckexception.h       \
    src/security/encodinghelper.h               \
    src/security/passwordhash.h                 \
    src/security/nosuchalgorithmexception.h     \
    src/security/passwordgenerateexception.h    \
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
    src/security/masterpasswordchecker.h        \
    src/security/configpasswordchecker.h        \
    src/randompassword.h                        \
    src/smartcard/cardexception.h               \
    src/smartcard/memorycard.h                  \
    src/util/timerstatusmessage.h               \
    src/singleapplication.h                     \
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

CONFIG     += warn_on qt exceptions

# -----------------------------------------------------------------------------

DEFINES    += VERSION=\"$$VERSION\"

isEmpty (debug) {
  #DEFINES  += QT_NO_CHECK
  CONFIG    += release
} else {
  CONFIG   += debug
  DEFINES  += DEBUG
}

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

