# Id: $Id: qpamat.pro,v 1.11 2003/11/16 20:22:04 bwalle Exp $
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
    src/cipher/encodinghelper.cpp               \
    src/cipher/passwordhash.cpp                 \
    src/cipher/encryptor.cpp                    \
    src/cipher/randompasswordgenerator.cpp      \
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
    src/cipher/encodinghelper.h                 \
    src/cipher/passwordhash.h                   \
    src/cipher/nosuchalgorithmexception.h       \
    src/cipher/notseededexception.h             \
    src/cipher/encryptor.h                      \
    src/cipher/randompasswordgenerator.h        \
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

