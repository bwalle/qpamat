# Id: $Id: qpamat.pro,v 1.18 2003/12/16 22:50:20 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
################################################################################


# set this if you installed OpenSSL in a non-standard place
# INCLUDEPATH += /usr/local/ssl/include

SOURCES     =                                   \
    src/dialogs/passworddialog.cpp              \
    src/dialogs/newpassworddialog.cpp           \
    src/dialogs/configurationdialog.cpp         \
    src/dialogs/randompassworddialog.cpp        \
    src/widgets/filelineedit.cpp                \
    src/widgets/fontchoosebox.cpp               \
    src/widgets/copylineedit.cpp                \
    src/widgets/focuslineedit.cpp               \
    src/security/encodinghelper.cpp             \
    src/security/passwordhash.cpp               \
    src/security/abstractencryptor.cpp          \
    src/security/symmetricencryptor.cpp         \
    src/security/notencryptor.cpp               \
    src/security/collectencryptor.cpp           \
    src/security/randompasswordgenerator.cpp    \
    src/security/passwordgeneratorfactory.cpp   \
    src/security/simplepasswordchecker.cpp      \
    src/security/extendedpasswordchecker.cpp    \
    src/security/extendeddictpasswordchecker.cpp\
    src/security/externalpasswordchecker.cpp    \
    src/security/passwordcheckerfactory.cpp     \
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
    src/widgets/copylineedit.h                  \
    src/widgets/focuslineedit.h                 \
    src/global.h                                \
    src/security/passwordcheckexception.h       \
    src/security/encodinghelper.h               \
    src/security/passwordhash.h                 \
    src/security/nosuchalgorithmexception.h     \
    src/security/notseededexception.h           \
    src/security/encryptor.h                    \
    src/security/notencryptor.h                 \
    src/security/abstractencryptor.h            \
    src/security/symmetricencryptor.h           \
    src/security/collectencryptor.h             \
    src/security/passwordgenerator.h            \
    src/security/randompasswordgenerator.h      \
    src/security/passwordgeneratorfactory.h     \
    src/security/passwordchecker.h              \
    src/security/simplepasswordchecker.h        \
    src/security/extendedpasswordchecker.h      \
    src/security/extendeddictpasswordchecker.h  \
    src/security/externalpasswordchecker.h      \
    src/security/passwordcheckerfactory.h       \
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

DEFINES    += VERSION=\"0.1\"

isEmpty (debug) {
  DEFINES  += QT_NO_CHECK
} else {
  CONFIG   += debug
  DEFINES  += DEBUG
 # LIBS     += -lefence
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

