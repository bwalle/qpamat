# (c) 2008, Bernhard Walle <bernhard.walle@gmx.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

PROJECT(qpamat)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(FindOpenSSL)
INCLUDE(FindQt4)
SET(QT_USE_QT3SUPPORT 1)
SET(QT_USE_QTXML      1)
IF (CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
    SET(QT_USE_QTDBUS     1)
ENDIF (CMAKE_HOST_UNIX AND NOT CMAKE_HOST_APPLE)
INCLUDE(${QT_USE_FILE})

# include the current working directory
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

# version
SET(VERSION_STRING  "0.5.4")
SET(MAJOR_VERSION   0)
SET(MINOR_VERSION   5)
SET(PATCH_VERSION   4)
ADD_DEFINITIONS(-DVERSION_STRING="${VERSION_STRING}")
ADD_DEFINITIONS(-DMAJOR_VERSION=${MAJOR_VERSION})
ADD_DEFINITIONS(-DMINOR_VERSION=${MINOR_VERSION})
ADD_DEFINITIONS(-DPATCH_VERSION=${PATCH_VERSION})

# always print compiler warnings
IF (CMAKE_HOST_UNIX)
    ADD_DEFINITIONS(-Wall -W -Wno-unused-parameter)
ENDIF (CMAKE_HOST_UNIX)

SET(qpamat_SRCS
    src/dialogs/passworddialog.cpp
    src/dialogs/newpassworddialog.cpp
    src/dialogs/configurationdialog.cpp
    src/dialogs/showpassworddialog.cpp
    src/dialogs/waitdialog.cpp
    src/dialogs/insertcarddialog.cpp
    src/dialogs/cardpinvalidator.cpp
    src/dialogs/aboutdialog.cpp
    src/widgets/filelineedit.cpp
    src/widgets/fontchoosebox.cpp
    src/widgets/copylabel.cpp
    src/widgets/focuslineedit.cpp
    src/widgets/listboxlabeledpict.cpp
    src/widgets/listboxdialog.cpp
    src/security/encodinghelper.cpp
    src/security/passwordhash.cpp
    src/security/abstractencryptor.cpp
    src/security/symmetricencryptor.cpp
    src/security/collectencryptor.cpp
    src/security/randompasswordgenerator.cpp
    src/security/externalpasswordgenerator.cpp
    src/security/passwordgeneratorfactory.cpp
    src/security/passwordchecker.cpp
    src/security/hybridpasswordchecker.cpp
    src/security/masterpasswordchecker.cpp
    src/smartcard/cardexception.cpp
    src/smartcard/memorycard.cpp
    src/smartcard/nosuchlibraryexception.cpp
    src/smartcard/notinitializedexception.cpp
    src/util/stringdisplay.cpp
    src/util/singleapplication.cpp
    src/util/timeoutapplication.cpp
    src/datareadwriter.cpp
    src/timerstatusmessage.cpp
    src/randompassword.cpp
    src/treeentry.cpp
    src/property.cpp
    src/tree.cpp
    src/settings.cpp
    src/qpamat.cpp
    src/help.cpp
    src/rightpanel.cpp
    src/rightlistview.cpp
    src/southpanel.cpp
    src/main.cpp
)

SET(qpamat_MOCS
    src/dialogs/passworddialog.h
    src/dialogs/configurationdialog.h
    src/dialogs/insertcarddialog.h
    src/dialogs/newpassworddialog.h
    src/dialogs/aboutdialog.h
    src/dialogs/showpassworddialog.h
    src/dialogs/configurationdialogprivate.h
    src/util/singleapplication.h
    src/util/timeoutapplication.h
    src/widgets/listboxdialog.h
    src/widgets/filelineedit.h
    src/widgets/fontchoosebox.h
    src/widgets/focuslineedit.h
    src/widgets/copylabel.h
    src/randompassword.h
    src/southpanel.h
    src/timerstatusmessage.h
    src/rightlistview.h
    src/tree.h
    src/rightpanel.h
    src/property.h
    src/treeentry.h
    src/help.h
    src/qpamat.h
)

# build some files only on specific platforms 
IF (CMAKE_HOST_UNIX)
    SET(qpamat_SRCS ${qpamat_SRCS} src/util/processinfo_unix.cpp)
    IF (NOT CMAKE_HOST_APPLE)
        SET(qpamat_SRCS ${qpamat_SRCS} src/qpamatadaptor.cpp)
        SET(qpamat_MOCS ${qpamat_MOCS} src/qpamatadaptor.h)
    ENDIF (NOT CMAKE_HOST_APPLE)
ENDIF (CMAKE_HOST_UNIX)

IF (CMAKE_HOST_WIN32)
    SET(qpamat_SRCS ${qpamat_SRCS} src/util/processinfo_win.cpp)
ENDIF (CMAKE_HOST_WIN32)

# translations
QT4_CREATE_TRANSLATION(
    qpamat_qmfile
    ${CMAKE_SOURCE_DIR}/src
    ts/de.ts
)

# Compiled-in pixmaps
SET(QtApp_RCCS qpamat.qrc)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(qpamat_RCC_SRCS qpamat.qrc)

# generate rules for building source files that moc generates
QT4_WRAP_CPP(qpamat_MOC_SRCS ${qpamat_MOCS})

# build sources, moc'd sources, and rcc'd sources
ADD_EXECUTABLE(qpamat
    ${qpamat_SRCS} ${qpamat_MOC_SRCS} ${qpamat_RCC_SRCS} ${qpamat_qmfile}
)

TARGET_LINK_LIBRARIES (qpamat ${QT_LIBRARIES} ${OPENSSL_LIBRARIES})

#
# Build the Documentation
#

FIND_PROGRAM(XSLTPROC_EXECUTABLE xsltproc)

# create target directories
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/user/en)
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/doc/user/en/qpamat.css
    ${CMAKE_BINARY_DIR}/doc/user/en/qpamat.css
    COPYONLY
)

ADD_CUSTOM_COMMAND(
    OUTPUT
        ${CMAKE_BINARY_DIR}/doc/user/en/index.html # creates more files
    COMMAND
        ${XSLTPROC_EXECUTABLE}
        ${CMAKE_SOURCE_DIR}/doc/user/en/qpamat.xsl
        ${CMAKE_SOURCE_DIR}/doc/user/en/qpamat.xml
    DEPENDS
        ${CMAKE_SOURCE_DIR}/doc/user/en/qpamat.xml
    WORKING_DIRECTORY
        ${CMAKE_BINARY_DIR}/doc/user/en
)

ADD_CUSTOM_TARGET(
    doc
    DEPENDS
        ${CMAKE_BINARY_DIR}/doc/user/en/index.html
)

ADD_DEPENDENCIES(
    qpamat
    doc
)


#
# Installation
#

INSTALL(
    TARGETS
        qpamat
    DESTINATION
        bin
)

INSTALL(
    FILES
        ${qpamat_qmfile}
    DESTINATION
        share/qpamat/translations
)

INSTALL(
    FILES
        ${CMAKE_SOURCE_DIR}/share/dicts/all.txt
        ${CMAKE_SOURCE_DIR}/share/dicts/english.txt
        ${CMAKE_SOURCE_DIR}/share/dicts/french.txt
        ${CMAKE_SOURCE_DIR}/share/dicts/german.txt
        ${CMAKE_SOURCE_DIR}/share/dicts/netherlands.txt
        ${CMAKE_SOURCE_DIR}/share/dicts/default.txt
    DESTINATION
        share/qpamat/dicts/
)

INSTALL(
    DIRECTORY
        ${CMAKE_BINARY_DIR}/doc/user/en
    DESTINATION
        share/qpamat/doc/
    FILES_MATCHING REGEX ".*.(html|css)"
)

# :tabSize=4:noTabs=true:indentSize=4:mode=cmake:
