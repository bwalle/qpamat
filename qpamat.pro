# Id: $Id: qpamat.pro,v 1.4 2003/10/03 18:17:34 bwalle Exp $
# -----------------------------------------------------------------------------

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

DEFINES    += VERSION="0.1" QT_NO_CHECK 

# -----------------------------------------------------------------------------

LIBS       += -lssl

# -----------------------------------------------------------------------------

CONFIG     += release warn_on qt exceptions

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

DEFINE     +=

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = doc
documentation.commands      = doxygen qpamat.doxy
QMAKE_EXTRA_UNIX_TARGETS   += documentation

