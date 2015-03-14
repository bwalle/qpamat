/*
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; You may only use
 * version 2 of the License, you have no option to use any other version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * -------------------------------------------------------------------------------------------------
 */
#include <ctime>
#include <cstdlib>

#include <QThread>
#include <QFile>
#include <QApplication>
#include <QCursor>
#include <QTimer>
#include <QEventLoop>
#include <QMessageBox>
#include <QFileInfo>
#include <QTranslator>
#include <QDateTime>
#include <QPixmap>
#include <QTextStream>
#include <QDebug>
#include <QScopedPointer>

#include "qpamatwindow.h"
#include "qpamat.h"
#include "datareadwriter.h"
#include "security/passwordhash.h"
#include "security/symmetricencryptor.h"
#include "dialogs/waitdialog.h"
#include "global.h"

/**
 * @class ReadWriteException
 *
 * @brief Exception that is thrown if something got wrong with reading or writing.
 *
 * This exception is thrown by the DataReadWriter class. It simply contains a
 * translated error message. You can display this error message in a
 * QMessageBox. There's also a severity which indicates the severity level and
 * which you can use to determine the message type.
 *
 * This class is a subclass of the the std::runtime_error exception class.
 * This is for convenience (catching and use the std::exception::what()
 * method). The message string may be localized but const char* works only in
 * ISO-8859-1 charset properly. Use the getMessage() method instead.
 *
 * @ingroup gui
 */

/**
 * @enum ReadWriteException::Severity
 *
 * Indicates the severity of a ReadWriteException. Use @c WARNING for not so critical
 * errors and @c CRITICAL for critical errors.
 */

/**
 * @enum ReadWriteException::Category
 *
 * @brief Indicates the category of the error.
 *
 * Maybe the reaction depends on the category of the error (for example if the
 * password was wrong we need to show the password dialog again).
 */

/**
 * @var ReadWriteException::CWrongPassword
 *
 * @brief Indicates a wrong password.
 *
 * The user has to enter a new one.
 */

/**
 * @var ReadWriteException::CInvalidData
 *
 * @brief Indicates invalid data such as a malformed XML file.
 *
 * Retrying makes no sense here.
 */

/**
 * @var ReadWriteException::CIOError
 *
 * Indicates an error with input/output such as wrong file permission.
 */

/**
 * @var ReadWriteException::CNoAlgorithm
 *
 * @brief Indicates that the algorithm for encryption does not exist.
 *
 * Retrying makes no sense since the user has to re-build OpenSSL and that
 * takes a bit ...
 */

/**
 * @var ReadWriteException::COtherError
 *
 * @brief The default: we don't know the category.
 *
 * This is always bad because we cannot react properly. Avoid this!
 */

/**
 * @fn ReadWriteException::CAbort
 *
 * @brief User wants to abort the action.
 */

/**
 * @fn ReadWriteException::ReadWriteException(const QString&, Category, Severity)
 *
 * @brief Creates a new instance of a ReadWriteException.
 *
 * @param error the error message which should be localized
 * @param category the category of the error
 * @param severity the severity which indicates the dialog type used later in the GUI
 */

/**
 * @fn ReadWriteException::retryMakesSense() const
 *
 * @brief Returns whether immediately retrying makes sense.
 *
 * @return @c true if it makes sense, @c false otherwise
 */

/**
 * @fn ReadWriteException::~ReadWriteException
 *
 * Destroys the ReadWriteException object.
 */

/**
 * @fn ReadWriteException::getMessage() const
 *
 * Returns the error message.
 *
 * @return the localized string, may contain HTML-tags
 */

/**
 * @fn ReadWriteException::getSeverity() const
 *
 * Returns the severity.
 * @return the severity
 */

/**
 * @fn ReadWriteException::getCategory() const
 *
 * Returns the category.
 * @return the category
 */

// ------------------------------------------------------------------------------------------------

/**
 * @class DataReadWriter
 *
 * @brief Handles the reading and writing from and to the file.
 *
 * This class handles reading and writing to the XML file. The input or
 * output is a XML structure with passwords as cleartext. This class does also the
 * encryption or decryption.
 *
 * It reads the current configuration from the global Settings object. The configuration
 * is the file, the encryption algorithm and so on.
 *
 * On error, a ReadWriteException is thrown and the error message is set to a sensible
 * value. It displays no error dialog itself, you have to to this on the calling part.
 *
 * No automatic delection takes place. This is no QObject.
 *
 * @par Writing
 *
 * In DOM, no XML element can exist without the context of a DomDocument. Therefore
 * you have to create an QDomDocument with the factory function createSkeletonDocument()
 * in this class. It gets filled with application-specific data and contains an empty
 * <tt>\<password\></tt> tag. The document must be passed to the Tree::writeToXML()
 * function.
 *
 * @bug PIN verification does not work here: I get 90 00 as response after verifying, but
 *       writing fails with 62 00 error !??
 *
 * @ingroup gui
 */


/**
 * @brief Creates a skeleton document that must be used for writing the XML tree to the
 *        harddisk.
 *
 * As described in the constructor documentation, it contains all application data.
 *
 * @return the QDomDocument object
 */
QDomDocument DataReadWriter::createSkeletonDocument()
{
    const char url[] = "http://qpamat.berlios.de/qpamat.dtd";
    QDomDocument doc(QDomImplementation().createDocumentType( "qpamat", 0, url));

    // add application-specific data
    QDomElement root = doc.createElement("qpamat");
    doc.appendChild(root);
    QDomElement appData = doc.createElement("app-data");
    root.appendChild(appData);

    QDomElement version = doc.createElement("version");
    QDomText versionValue = doc.createTextNode(VERSION_STRING);
    version.appendChild(versionValue);
    appData.appendChild(version);

    QDomElement date = doc.createElement("date");
    QDomText algo = doc.createTextNode(QDateTime::currentDateTime(Qt::UTC).toString(Qt::ISODate));
    date.appendChild(algo);
    appData.appendChild(date);

    QDomElement cryptAlgorithm = doc.createElement("crypt-algorithm");
    QpamatWindow *win = Qpamat::instance()->getWindow();
    QDomText algorithm = doc.createTextNode(win->set().readEntry("Security/CipherAlgorithm"));
    cryptAlgorithm.appendChild(algorithm);
    appData.appendChild(cryptAlgorithm);

    QDomElement passwordhash = doc.createElement("passwordhash");
    appData.appendChild(passwordhash);

    QDomElement smartcard = doc.createElement("smartcard");
    smartcard.setAttribute("useCard", false);
    appData.appendChild(smartcard);

    // add the empty passwords child
    QDomElement passwords = doc.createElement("passwords");
    root.appendChild(passwords);

    return doc;
}

// -------------------------------------------------------------------------------------------------


/**
 * @brief Writes the document in the file specified in the global settings.
 *
 * Encryption is done before writing with the specified password. If something went wrong,
 * a ReadWriteException is thrown.
 *
 * @param document the XML document to write
 * @param password the password which is used for encryption
 * @exception ReadWriteException several reasons
 *               - file could not be opened
 *               - cipher algorithm is not available
 *               - error in communicating with the smart-card terminal
 */
void DataReadWriter::writeXML(const QDomDocument& document, const QString& password)
{
    QDomDocument document_cpy = document.cloneNode(true).toDocument();
    QpamatWindow *win = Qpamat::instance()->getWindow();
    const QString fileName = win->set().readEntry("General/Datafile");
    const QString algorithm = win->set().readEntry("Security/CipherAlgorithm");

    // check if the file can be added
    QFile file(fileName);

    if (QFileInfo(file).exists() && !QFileInfo(file).isWritable())
        throw ReadWriteException(QObject::tr("<qt><nobr>The data file is not writable. Change "
            "the file in</nobr> the configuration dialog or change the permission of the file!"
            "</qt>"), ReadWriteException::CIOError);

    // set up the needed encryptors
    QScopedPointer<StringEncryptor> enc;
    QScopedPointer<Encryptor> realEncryptor;
    try {
        enc.reset(new SymmetricEncryptor(algorithm, password));
    }
    catch (const NoSuchAlgorithmException&)
    {
        throw ReadWriteException(QObject::tr("The algorithm '%1' is not avaible on "
            "your system.\nChoose another crypto algorithm in the settings.\nThe data "
            "is not saved!").arg(algorithm), ReadWriteException::CNoAlgorithm);
    }

    // encrypt
    QDomElement pwData = document_cpy.documentElement().namedItem("passwords").toElement();
    QDomElement appData = document_cpy.documentElement().namedItem("app-data").toElement();
    crypt(pwData, *enc, true);

    // write the password hash
    const QString hash = PasswordHash::generateHashString(password);
    QDomText text = document_cpy.createTextNode(hash);
    appData.namedItem("passwordhash").toElement().appendChild(text);

    if (!file.open(QIODevice::WriteOnly))
        throw ReadWriteException(QObject::tr("The data could not be saved. There "
            "was an\nerror while creating the file:\n%1").arg( qApp->translate("QFile",
            file.errorString())), ReadWriteException::CIOError);

    QTextStream stream(&file);
    stream.setEncoding(QTextStream::UnicodeUTF8);
    stream << document_cpy.toString();
}


/**
 * @brief Reads the document from the specified XML (global settings) file and decrypts the
 *        passwords using the given \p password.
 *
 * It does also a password check.
 *
 * @param password the decryption password
 * @return the document
 * @exception ReadWriteException several reasons
 *               - cannot open the XML file
 *               - invalid XML file
 *               - wrong password (checked with the checksum stored in the file or on
 *                                 the chipcard)
 *               - wrong configuration of the smartcard terminal
 *               - algorithm does not exist in this OpenSSL configuration
 *               - error with communicating with the card terminal
 */
QDomDocument DataReadWriter::readXML(const QString& password)
{
    qDebug() << CURRENT_FUNCTION;

    QpamatWindow *win = Qpamat::instance()->getWindow();
    const QString& fileName = win->set().readEntry("General/Datafile");

    // load the XML structure
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        throw ReadWriteException(QObject::tr("The file %1 could not be opened:\n%2.").
            arg(fileName).arg(qApp->translate("QFile", file.errorString())),
            ReadWriteException::CIOError);

    QDomDocument doc;
    if (!doc.setContent(&file))
        throw ReadWriteException(QObject::tr("The XML file (%1) may be corrupted "
            "and\ncould not be read. Check the file with a text editor.").arg(fileName),
            ReadWriteException::CInvalidData);

    file.close();
    QDomElement appData = doc.documentElement().namedItem("app-data").toElement();

    if (appData.namedItem("smartcard").toElement().attribute("useCard").toInt())
        throw ReadWriteException(QObject::tr("<qt><nobr>SmartCard support has been removed in QPaMaT 0.6.0. You need "
            "to store the passwords in the file using an old version of QPaMaT.</qt>"),
            ReadWriteException::CConfigurationError);

    // check the password
    const QString hash = appData.namedItem("passwordhash").toElement().text();
    if (hash == "SMARTCARD" || !PasswordHash::isCorrect(password, hash))
        throw ReadWriteException(QObject::tr("The password is incorrect."),
            ReadWriteException::CWrongPassword);

    QString algorithm = appData.namedItem("crypt-algorithm").toElement().text();
    QScopedPointer<StringEncryptor> enc;
    QScopedPointer<Encryptor> realEncryptor;
    try {
        enc.reset(new SymmetricEncryptor(algorithm, password));
    } catch (const NoSuchAlgorithmException& ex) {
        UNUSED(ex);
        throw ReadWriteException(QObject::tr("The algorithm '%1' is not avaible on "
                "your system.\nIt is impossible to read the file. Try to recompile or\n"
                "update your OpenSSL library.").arg(algorithm), ReadWriteException::CNoAlgorithm);
    }

    QDomElement pwData = doc.documentElement().namedItem("passwords").toElement();
    crypt(pwData, *enc, false);

    return doc;
}


void DataReadWriter::crypt(QDomElement& n, StringEncryptor& enc, bool encrypt)
{
    QDomNodeList list = n.childNodes();
    if (list.length() != 0) {
        for (unsigned int i = 0; i < list.length(); ++i) {
            QDomElement n = list.item(i).toElement();
            crypt(n, enc, encrypt);
        }
    } else {
        QDomElement el = n.toElement();
        if (!el.isNull()) {
            if (el.tagName() == "property" && el.attribute("type") == "PASSWORD"
                    && el.hasAttribute("value")) {
                if (encrypt)
                    el.setAttribute("value", enc.encryptStrToStr(el.attribute("value")));
                else
                    el.setAttribute("value", enc.decryptStrFromStr(el.attribute("value")));
            }
        }
    }
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
