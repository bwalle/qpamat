# Id: $Id: qpamat.pro,v 1.3 2003/09/21 16:02:09 bwalle Exp $
# -----------------------------------------------------------------------------

SOURCES     =                                   \
    src/main.cpp                                \
    src/encodinghelper.cpp                      \
    src/cipher/encryptor.cpp                    \
    src/cipher/randompasswordgenerator.cpp      

# -----------------------------------------------------------------------------

HEADERS     =                                   \
    src/types.h                                 \
    src/encodinghelper.h                        \
    src/cipher/nosuchalgorithmexception.h       \
    src/cipher/notseededexception.h             \
    src/cipher/encryptor.h                      \
    src/cipher/randompasswordgenerator.h        

# -----------------------------------------------------------------------------

DEFINES    += VERSION="0.1"

# -----------------------------------------------------------------------------

LIBS       += -lssl

# -----------------------------------------------------------------------------

CONFIG     += debug warn_on qt exceptions

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

