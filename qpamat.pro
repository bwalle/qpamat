# Id: $Id: qpamat.pro,v 1.12 2003/11/28 18:42:55 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
################################################################################

SOURCES     =                                   \
    src/dialogs/passworddialog.cpp              \
    src/dialogs/newpassworddialog.cpp           \
    src/dialogs/configurationdialog.cpp         \
    src/widgets/filelineedit.cpp                \
    src/security/encodinghelper.cpp             \
    src/security/passwordhash.cpp               \
    src/security/encryptor.cpp                  \
    src/security/randompasswordgenerator.cpp    \
    src/smartcard/cardexception.cpp             \
    src/smartcard/memorycard.cpp                \
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
    src/widgets/filelineedit.h                  \
    src/types.h                                 \
    src/security/encodinghelper.h               \
    src/security/passwordhash.h                 \
    src/security/nosuchalgorithmexception.h     \
    src/security/notseededexception.h           \
    src/security/encryptor.h                    \
    src/security/randompasswordgenerator.h      \
    src/smartcard/cardexception.h               \
    src/smartcard/memorycard.h                  \
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

LIBS       += -lssl -lm 

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = doc
documentation.commands      = doxygen qpamat.doxy
QMAKE_EXTRA_UNIX_TARGETS   += documentation

