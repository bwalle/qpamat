# Id: $Id: qpamat.pro,v 1.46 2004/07/23 15:11:22 bwalle Exp $
# -----------------------------------------------------------------------------

#
#  Use "qmake -o Makefile qpamat.pro" for release code, and
#  use "qmake -o Makefile qpamat.pro debug=1" for debug code.
#
#  Adapt these variables to your system

PREFIX             = /usr/local/
SHAREDIR           = $${PREFIX}/share
DOCDIR             = $${SHAREDIR}/doc/packages
BINDIR             = $${PREFIX}/bin

# set this if you installed OpenSSL in a non-standard place
# INCLUDEPATH += /usr/local/ssl/include

################################################################################

VERSION_STRING     = 0.3.0
MAJOR_VERSION      = 0
MINOR_VERSION      = 3
PATCH_VERSION      = 0

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
IMAGES =                                        \
    images/stock_print_16.png                   \
    images/stock_print_24.png                   \
    images/stock_exit_16.png                    \
    images/stock_exit_24.png                    \
    images/stock_new_16.png                     \
    images/stock_new_24.png                     \
    images/stock_save_16.png                    \
    images/stock_save_24.png                    \             
    images/stock_preferences_16.png             \
    images/stock_preferences_24.png             \
    images/stock_search_16.png                  \
    images/stock_search_24.png                  \
    images/stock_up_arrow_16.png                \
    images/stock_up_arrow_24.png                \
    images/stock_down_arrow_16.png              \
    images/stock_down_arrow_24.png              \
    images/stock_copy_16.png                    \
    images/stock_copy_24.png                    \
    images/stock_font_16.png                    \
    images/stock_font_24.png                    \
    images/stock_add_16.png                     \
    images/stock_add_24.png                     \
    images/stock_remove_16.png                  \
    images/stock_remove_24.png                  \
    images/stock_help_16.png                    \
    images/stock_help_24.png                    \
    images/stock_directory_16.png               \
    images/clear_clipboard_16.png               \
    images/general_34.png                       \
    images/password_34.png                      \
    images/stock_dialog_authentication_48.png   \
    images/stock_dialog_authentication_34.png   \
    images/presentation_34.png                  \
    images/traffic_gray_16.png                  \
    images/traffic_out_22.png                   \
    images/traffic_red_16.png                   \
    images/traffic_red_22.png                   \
    images/traffic_green_16.png                 \
    images/traffic_green_22.png                 \
    images/traffic_yellow_16.png                \
    images/traffic_yellow_22.png                \
    images/rename_16.png                        \
    images/smartcard_24.png                     \
    images/smartcard_34.png                     \
    images/smartcard_48.png                     \
    images/whats_this.png                       \
    images/qt_16.png                            \
    images/eye_16.png

# -----------------------------------------------------------------------------

isEmpty(static) {
  CONFIG     += warn_on qt exceptions
} else {
  LIBS        += /usr/lib/qt3/lib/libqt-mt.a -lfreetype -lXft -lz -lpthread \
                 -lpng -lX11 -lXext -lXinerama -lXi -lSM -lXcursor -lXrandr -lGL
  CONFIG      += x11 warn_on exceptions
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

QMAKE_CXXFLAGS_WARN_ON 	+= -Wno-unused-parameter

# -----------------------------------------------------------------------------

LIBS       += -lssl -lm -lcrypto

# -----------------------------------------------------------------------------

MOC_DIR     = out/
OBJECTS_DIR = out/

# -----------------------------------------------------------------------------

# build the documentation with "make doc"
# (Unix only, run "doxygen qpamant.doxy" on Microsoft Windows)
documentation.target        = documentation
documentation.commands      = doxygen qpamat.doxy
QMAKE_EXTRA_UNIX_TARGETS   += documentation

#
# make a tarball on Unix with "make release"
maketarball.target          = tarball
maketarball.commands        = cd .. &&
maketarball.commands       += cp -r qpamat qpamat-$$VERSION_STRING &&
maketarball.commands       += rm qpamat-$$VERSION_STRING/out/* &&
maketarball.commands       += rm qpamat-$$VERSION_STRING/qpamat &&
maketarball.commands       += find qpamat-$$VERSION_STRING -name CVS -type d -exec rm -r {} \; ;
maketarball.commands       += find qpamat-$$VERSION_STRING -name '*.o' -exec rm {} \; ;
maketarball.commands       += find qpamat-$$VERSION_STRING -name '*.qm' -exec rm {} \; ;
maketarball.commands       += find qpamat-$$VERSION_STRING -name .cvsignore -exec rm {} \; ;
maketarball.commands       += tar cvfz qpamat-$${VERSION_STRING}.tar.gz qpamat-$$VERSION_STRING &&
maketarball.commands       += cd - &&
maketarball.commands       += mv ../qpamat-$${VERSION_STRING}.tar.gz . &&
maketarball.commands       += rm -r ../qpamat-$$VERSION_STRING
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

i_messages.path             = $$SHAREDIR/qpamat/translations
i_messages.files           += ts/*.qm
INSTALLS                   += i_messages

target.path                 = $$BINDIR
INSTALLS                   += target

# vim: ft=text et
