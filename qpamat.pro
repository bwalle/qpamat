# Id: $Id: qpamat.pro,v 1.2 2003/09/20 13:38:41 bwalle Exp $
# -----------------------------------------------------------------------------

SOURCES     =                                   \
    src/main.cpp                                \
    src/encodinghelper.cpp                      \
    src/encrypt/nosuchalgorithmexception.cpp    

# -----------------------------------------------------------------------------

HEADERS     =                                   \
    src/types.h                                 \
    src/encodinghelper.h                        \
    src/encrypt/nosuchalgorithmexception.h

# -----------------------------------------------------------------------------

DEFINES    += VERSION="0.1"

# -----------------------------------------------------------------------------

LIBS       += -lssl

# -----------------------------------------------------------------------------

CONFIG     += debug warn_on qt exceptions

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

