# Id: $Id: qpamat.pro,v 1.5 2003/10/03 18:56:02 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
################################################################################

SOURCES     =                                   \
    src/main.cpp                                \
    src/encodinghelper.cpp                      \
    src/cipher/encryptor.cpp                    \
    src/cipher/randompasswordgenerator.cpp      \
    src/treeentry.cpp                           \
    src/property.cpp

# -----------------------------------------------------------------------------

HEADERS     =                                   \
    src/types.h                                 \
    src/encodinghelper.h                        \
    src/cipher/nosuchalgorithmexception.h       \
    src/cipher/notseededexception.h             \
    src/cipher/encryptor.h                      \
    src/cipher/randompasswordgenerator.h        \
    src/treeentry.h                             \
    src/property.h

# -----------------------------------------------------------------------------

CONFIG     += warn_on qt exceptions

# -----------------------------------------------------------------------------

DEFINES    += VERSION="0.1"

isEmpty (debug) {
  DEFINES  += QT_NO_CHECK
} else {
  CONFIG   += debug
}

# -----------------------------------------------------------------------------

LIBS       += -lssl

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = doc
documentation.commands      = doxygen qpamat.doxy
QMAKE_EXTRA_UNIX_TARGETS   += documentation

