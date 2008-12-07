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
#include <memory>
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

#include "qpamat.h"
#include "datareadwriter.h"
#include "smartcard/memorycard.h"
#include "security/passwordhash.h"
#include "security/symmetricencryptor.h"
#include "security/collectencryptor.h"
#include "dialogs/insertcarddialog.h"
#include "dialogs/waitdialog.h"
//#include "treeentry.h"

#include "global.h"

/*!
    \class ReadWriteException

    \brief Exception that is thrown if something got wrong with reading or writing.

    This exception is thrown by the DataReadWriter class. It simply contains a translated
    error message. You can display this error message in a QMessageBox. There's also a
    severity which indicates the severity level and which you can use to determine the
    message type. 

    This class is a subclass of the the std::runtime_error exception class. This is for
    convenience (catching and use the std::exception::what() method). The message string
    may be localized but const char* works only in ISO-8859-1 charset properly. Use the
    getMessage() method instead.

    \ingroup gui
    \author $Author$
    \version $Revision: 1.8 $
    \date $Date$
    
*/

/*!
    \enum ReadWriteException::Severity  

    Indicates the severity of a ReadWriteException. Use \c WARNING for not so critical
    errors and \c CRITICAL for critical errors.
*/

/*!
    \enum ReadWriteException::Category

    Indicates the category of the error. Maybe the reaction depends on the category of
    the error (for example if the password was wrong we need to show the password
    dialog again).
*/

/*!
    \var ReadWriteException::CWrongPassword

    Indicates a wrong password. The user has to enter a new one.
*/
/*!
    \var ReadWriteException::CInvalidData

    Indicates invalid data such as a malformed XML file. Retrying makes no sense here.
*/
/*!
    \var ReadWriteException::CIOError

    Indicates an error with input/output such as wrong file permission.
*/
/*!
    \var ReadWriteException::CSmartcardError

    Indicates an error while communicating with the smartcard terminal. Retrying
    makes heavily sense here because maybe the user just forgot to insert the card
    and pressed Ok.
*/
/*!
    \var ReadWriteException::CNoAlgorithm

    Indicates that the algorithm for encryption does not exist. Retrying makes
    no sense since the user has to re-build OpenSSL and that takes a bit ...
*/
/*!
    \var ReadWriteException::COtherError

    The default: we don't know the category. This is always bad because we cannot
    react properly. Avoid this!
*/
/*!
    \fn ReadWriteException::CAbort
    
    User wants to abort the action.
*/

/*!
    \fn ReadWriteException::ReadWriteException(const QString&, Category, Severity)

    Creates a new instance of a ReadWriteException. 
    \param error the error message which should be localized
    \param category the category of the error
    \param severity the severity which indicates the dialog type used later in the GUI
*/

/*!
    \fn ReadWriteException::retryMakesSense() const
    
    Returns whether immediately retrying makes sense.
    \return \c true if it makes sense, \c false otherwise
*/

/*!
    \fn ReadWriteException::~ReadWriteException

    Destroys the ReadWriteException object.
*/

/*!
    \fn ReadWriteException::getMessage() const

    Returns the error message.
    \return the localized string, may contain HTML-tags
*/

/*!
    \fn ReadWriteException::getSeverity() const

    Returns the severity.
    \return the severity
*/

/*!
    \fn ReadWriteException::getCategory() const

    Returns the category.
    \return the category
*/

// ------------------------------------------------------------------------------------------------

/*!
    \class DataReadWriter

    \brief Handles the reading and writing from and to the smartcard and/or file.

    This class handles reading and writing to the XML file and the smartcard. The input
    or output is a XML structure with passwords as cleartext. This class does also the
    encryption or decryption.

    It reads the current configuration from the global Settings object. The configuration
    is the file, the smartcard settings, the encryption algorithm and so on.

    On error, a ReadWriteException is thrown and the error message is set to a
    sensible value. It displays no error dialog itself, you have to to this on the
    calling part.

    No automatic delection takes place. This is no QObject. We only need the parent
    widget to display a message box if the user should insert the smartcard.
    
    \par Writing
    
    In DOM, no XML element can exist without the context of a DomDocument. Therefore
    you have to create an QDomDocument with the factory function createSkeletonDocument()
    in this class. It gets filled with application-specific data and contains an empty
    <tt>\<password\></tt> tag. The document must be passed to the Tree::writeToXML()
    function.
    
    \bug PIN verification does not work here: I get 90 00 as response after verifying, but
          writing fails with 62 00 error !??

    \ingroup gui
    \author $Author$
    \version $Revision: 1.8 $
    \date $Date$
*/

/*!
    Creates a new instance of a DataReadWriter. The parent is needed for the message box
    which asks the user to insert the password.
    \param parent the parent widget
*/
DataReadWriter::DataReadWriter(QWidget* parent)
    : m_parent(parent)
{}


/*!
    Creates a skeleton document that must be used for writing the XML tree to the harddisk.
    As described in the constructor documentation, it contains all application data.
    \return the QDomDocument object
*/
QDomDocument DataReadWriter::createSkeletonDocument() throw ()
{
    const char url[] = "http://qpamat.berlios.de/qpamat.dtd";
    QDomDocument doc(QDomImplementation().createDocumentType( "qpamat", 0, url));
    
    // add application-specific data
    QDomElement root = doc.createElement("qpamat");
    doc.appendChild(root);
    QDomElement appData = doc.createElement("app-data");
    root.appendChild(appData);
    
    QDomElement version = doc.createElement("version");
    version.setAttribute("major", MAJOR_VERSION);
    version.setAttribute("minor", MINOR_VERSION);
    version.setAttribute("patch", PATCH_VERSION);
    appData.appendChild(version);
    
    QDomElement date = doc.createElement("date");
    QDomText algo = doc.createTextNode(QDateTime::currentDateTime(Qt::UTC).toString(Qt::ISODate));
    date.appendChild(algo);
    appData.appendChild(date);
    
    QDomElement cryptAlgorithm = doc.createElement("crypt-algorithm");
    QDomText algorithm = doc.createTextNode(qpamat->set().readEntry("Security/CipherAlgorithm"));
    cryptAlgorithm.appendChild(algorithm);
    appData.appendChild(cryptAlgorithm);
    
    QDomElement passwordhash = doc.createElement("passwordhash");
    appData.appendChild(passwordhash);
    
    QDomElement smartcard = doc.createElement("smartcard");
    smartcard.setAttribute("useCard", qpamat->set().readBoolEntry("Smartcard/UseCard"));
    appData.appendChild(smartcard);
    
    // add the empty passwords child
    QDomElement passwords = doc.createElement("passwords");
    root.appendChild(passwords);
    
    return doc;
}

// -------------------------------------------------------------------------------------------------
#ifndef DOXYGEN

class ReadWriteThread : public QThread
{
    public:
        ReadWriteThread(MemoryCard& card, ByteVector& bytes, bool write, byte& randomNumber,
            const QString& password, const QString& pin)
            : m_card(card), m_bytes(bytes), m_write(write), m_randomNumber(randomNumber), 
              m_password(password), m_exception(0), m_pin(pin) { }
            
        ~ReadWriteThread() 
            { delete m_exception; }
            
        ReadWriteException* getException() const 
            { return m_exception; }
    protected:
        void run();
    private:
        MemoryCard&         m_card;
        ByteVector&         m_bytes;
        const bool          m_write;
        byte&               m_randomNumber;
        const QString&      m_password;
        ReadWriteException* m_exception;
        const QString&      m_pin;
};

// -------------------------------------------------------------------------------------------------

/*!
    \class ReadWriteThread
    
    Thread that is responsible for reading and writing to the smart card. Because the real
    operations are long and atomar, the GUI would be blocked if the operations are not running
    in an own thread.
    
    No GUI operations take place in this thread. Instead of that, if an error occured the
    error message is set and the operation is finished. The caller has to check the error message
    and must display a message.
    
    The access to the variables which are passed to the constructor are not locked with a
    QMutex or something like that. The caller must ensure that he doesn't access this variables
    while this thread is running. This is usually no problem because the caller displays just
    a dialog which says the user that he must wait until the operation is finished.
*/

/*!
    \fn ReadWriteThread::ReadWriteThread(MemoryCard&, ByteVector&, bool, byte&, const QString&)
    
    Creates a new instance of a ReadWriteThread.
    \param card the memory card, it must be initialized with the right port (the reason is that
           the user should get a message box, insert the card and confirm the box while the
           memory card class should wait for it. So another process cannot access the card terminal
           at this time, this is important for security reasons
    \param bytes the read or write bytes
    \param write \c true if a write operation should be made, \c false for a read operation
    \param randomNumber the random number
    \param password the password to check
*/

/*!
    Runs the operation.
*/
void ReadWriteThread::run()
{
    try
    {
        if (m_card.getType() != MemoryCard::TMemoryCard)
        {
            m_exception = new ReadWriteException(QObject::tr("There's no memory card in your "
                "reader.\nUse the test function in the configuration\ndialog to set up your "
                "reader properly."), ReadWriteException::CSmartcardError);
            return;
        }
        
        if (m_write && !m_pin.isNull())
        {
            // try to unlock
            PRINT_DBG("Trying to unlock the card ...%s", "");
            m_card.verify(m_pin);
        }
        
        if (!m_card.selectFile())
        {
            m_exception = new ReadWriteException(QObject::tr("<qt>It was not possible to select the "
                "file on the smartcard</qt>"), ReadWriteException::CSmartcardError);
            return;
        }
        
        if (m_write)
        {
            // write the random number
            ByteVector byteVector(1);
            byteVector[0] = m_randomNumber;
            PRINT_DBG("Writing random = %d", byteVector[0])
            m_card.write(0, byteVector);
            
            // write the password hash and include a length information
            ByteVector pwHash = PasswordHash::generateHash(m_password);
            pwHash.insert(pwHash.begin(), pwHash.size());
            m_card.write(1, pwHash);
            
            PRINT_TRACE("Password hash length = %d", pwHash.size()-1);
            
            // then write the number of bytes
            int numberOfBytes = m_bytes.size();
            byteVector.resize(3);
            byteVector[0] = (numberOfBytes & 0xFF00) >> 8;
            byteVector[1] = numberOfBytes & 0xFF;
            byteVector[2] = 0; // fillbyte
            m_card.write(PasswordHash::MAX_HASH_LENGTH+2, byteVector);
            
            // and finally write the data
            m_card.write(PasswordHash::MAX_HASH_LENGTH+5, m_bytes);
            
        }
        else
        {
            // read the random number
            if (m_card.read(0, 1)[0] != m_randomNumber)
            {
                m_exception = new ReadWriteException(QObject::tr("You inserted the wrong smartcard!"),
                    ReadWriteException::CSmartcardError);
                return;
            }
            
            PRINT_DBG("Read randomNumber = %d", m_randomNumber)
            
            // read the password hash, check the password and throw a exception if necessary
            byte len = m_card.read(1, 1)[0];
            ByteVector pwHash = m_card.read(2, len);
            
            PRINT_TRACE("Password hash length = %d", len);
            
            if (!PasswordHash::isCorrect(m_password, pwHash))
            {
                m_exception = new ReadWriteException(QObject::tr("The given password was wrong."),
                    ReadWriteException::CWrongPassword);
                return;
            }

            // read the number
            ByteVector vec = m_card.read(PasswordHash::MAX_HASH_LENGTH + 2, 2);
            int numberOfBytes = (vec[0] << 8) + (vec[1]);
            
            PRINT_DBG("Read numberOfBytes = %d", numberOfBytes);

            Q_ASSERT(numberOfBytes >= 0);
            
            // read the bytes
            m_bytes = m_card.read(PasswordHash::MAX_HASH_LENGTH+5, numberOfBytes);
        }
    }
    catch (const CardException& e)
    {
        if (e.getErrorCode() == CardException::WrongVerification)
        {
            m_exception = new ReadWriteException(QObject::tr("<qt><nobr>The PIN you entered was wrong. "
                "You have</nobr> <b>%1</b> retries. After that, the card is destroyed!").arg(
                QString::number(e.getRetryNumber())), ReadWriteException::CSmartcardError);
        }
        else
        {
            m_exception = new ReadWriteException(QObject::tr("<qt><nobr>There was a communication error "
                "while</nobr> communicating with the smartcard terminal.<p>The error message was:"
                "<br><nobr>%1</nobr>").arg(e.what()), ReadWriteException::CSmartcardError);
        }
        return;
    }
}


/*!
    \fn ReadWriteThread::getException() const
    Returns the exception that occured or 0 if no exception occured. The pointer becomes invalid
    after the thread is deleted.
    \return the exception
*/

#endif // DOXYGEN

// -------------------------------------------------------------------------------------------------


/*!
    Writes the document in the file specified in the global settings. Encryption
    is done before writing with the specified password. If something went wrong, 
    a ReadWriteException is thrown.
    \param document the XML document to write
    \param password the password which is used for encryption
    \exception ReadWriteException several reasons
                 - file could not be opened
                 - cipher algorithm is not available
                 - error in communicating with the smart-card terminal
*/
void DataReadWriter::writeXML(const QDomDocument& document, const QString& password)
    throw (ReadWriteException)
{
    QDomDocument document_cpy = document.cloneNode(true).toDocument();
    bool smartcard = qpamat->set().readBoolEntry("Smartcard/UseCard");
    const QString fileName = qpamat->set().readEntry("General/Datafile");
    const QString algorithm = qpamat->set().readEntry("Security/CipherAlgorithm");
    
    // check if the file can be added    
    QFile file(fileName);
    
    if (QFileInfo(file).exists() && !QFileInfo(file).isWritable())
    {
        throw ReadWriteException(QObject::tr("<qt><nobr>The data file is not writable. Change "
            "the file in</nobr> the configuration dialog or change the permission of the file!"
            "</qt>"), ReadWriteException::CIOError);
    }
    
    // set up the needed encryptors
    std::auto_ptr<StringEncryptor> enc;
    std::auto_ptr<Encryptor> realEncryptor;
    try
    {
        if (smartcard)
        {
            realEncryptor = std::auto_ptr<Encryptor>(new SymmetricEncryptor(algorithm, password));
            enc = std::auto_ptr<StringEncryptor>(new CollectEncryptor(*realEncryptor));
        }
        else
        {
            enc = std::auto_ptr<StringEncryptor>(new SymmetricEncryptor(algorithm, password));
        }
    }
    catch (const NoSuchAlgorithmException& e)
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
    const QString hash = smartcard
        ? "SMARTCARD"
        : PasswordHash::generateHashString(password);
    QDomText text = document_cpy.createTextNode(hash);
    appData.namedItem("passwordhash").toElement().appendChild(text);
    
    byte id = 0;
    if (smartcard)
    {
        ByteVector vec = dynamic_cast<CollectEncryptor*>(enc.get())->getBytes();
        writeOrReadSmartcard(vec, true, id, password);
        appData.namedItem("smartcard").toElement().setAttribute("card-id", id);
    }
    
    if (!file.open(QIODevice::WriteOnly))
    {
        throw ReadWriteException(QObject::tr("The data could not be saved. There "
            "was an\nerror while creating the file:\n%1").arg( qApp->translate("QFile",
            file.errorString())), ReadWriteException::CIOError);
    }
    
    QTextStream stream(&file);
    stream.setEncoding(QTextStream::UnicodeUTF8);
    stream << document_cpy.toString();
}


/*!
    Reads the document from the specified XML (global settings) file and decrypts the
    passwords using the given \p password. It does also a password check.
    \param password the decryption password
    \return the document
    \exception ReadWriteException several reasons
                 - cannot open the XML file
                 - invalid XML file
                 - wrong password (checked with the checksum stored in the file or on
                                   the chipcard)
                 - wrong configuration of the smartcard terminal
                 - algorithm does not exist in this OpenSSL configuration
                 - error with communicating with the card terminal
*/
QDomDocument DataReadWriter::readXML(const QString& password)
    throw (ReadWriteException)
{
    PRINT_TRACE("Function called");

    const QString& fileName = qpamat->set().readEntry("General/Datafile");
    bool smartcard = qpamat->set().readBoolEntry("Smartcard/UseCard");
    
    // load the XML structure
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw ReadWriteException(QObject::tr("The file %1 could not be opened:\n%2.").
            arg(fileName).arg(qApp->translate("QFile", file.errorString())),
            ReadWriteException::CIOError);
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        throw ReadWriteException(QObject::tr("The XML file (%1) may be corrupted "
            "and\ncould not be read. Check the file with a text editor.").arg(fileName),
            ReadWriteException::CInvalidData);
    }
    file.close();
    QDomElement appData = doc.documentElement().namedItem("app-data").toElement();
    
    if (appData.namedItem("smartcard").toElement().attribute("useCard").toInt() && !smartcard)
    {
        throw ReadWriteException(QObject::tr("<qt><nobr>The passwords of the current data file"
            " are stored</nobr> on a smartcard but you did not configure QPaMaT for reading "
            "smartcards.<p>Change the settings and try again!</qt>"), 
            ReadWriteException::CConfigurationError);
    }
    smartcard = appData.namedItem("smartcard").toElement().attribute("useCard").toInt();
    
    // check the password
    if (!smartcard)
    {
        const QString hash = appData.namedItem("passwordhash").toElement().text();
        if (hash == "SMARTCARD" || !PasswordHash::isCorrect(password, hash))
        {
            throw ReadWriteException(QObject::tr("The password is incorrect."),
                ReadWriteException::CWrongPassword);
        }
    }
    
    
    QString algorithm = appData.namedItem("crypt-algorithm").toElement().text();
    std::auto_ptr<StringEncryptor> enc;
    std::auto_ptr<Encryptor> realEncryptor;
    try
    {
        if (smartcard)
        {
            realEncryptor = std::auto_ptr<Encryptor>(new SymmetricEncryptor(algorithm, password));
            enc = std::auto_ptr<StringEncryptor>(new CollectEncryptor(*realEncryptor));
        }
        else
        {
            enc = std::auto_ptr<SymmetricEncryptor>(new SymmetricEncryptor(algorithm, password));
        }
    }
    catch (const NoSuchAlgorithmException& ex)
    {
        throw ReadWriteException(QObject::tr("The algorithm '%1' is not avaible on "
                "your system.\nIt is impossible to read the file. Try to recompile or\n"
                "update your OpenSSL library.").arg(algorithm), ReadWriteException::CNoAlgorithm);
    }
        
    // read the data from the smartcard
    if (smartcard)
    {
        ByteVector vec;
        byte id = byte(appData.namedItem("smartcard").toElement().attribute("card-id").toShort());

        // also throws exception
        writeOrReadSmartcard(vec, false, id, password);
        dynamic_cast<CollectEncryptor*>(enc.get())->setBytes(vec);
    }
    
    QDomElement pwData = doc.documentElement().namedItem("passwords").toElement();
    crypt(pwData, *enc, false);
    

    return doc;
}


/*!
    Reads or writes from the smartcard. Displays an error message if needed. Refer to the
    ReadWriteThread for more information.
    \param bytes the bytes
    \param write reading or writing
    \param randomNumber the random number
*/
void DataReadWriter::writeOrReadSmartcard(ByteVector& bytes, bool write, byte& randomNumber, 
    const QString& password) throw (ReadWriteException)
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    
    // at first we need a random number 
    if (write)
    {
        std::srand(std::time(0));
        randomNumber = byte((double(std::rand())/RAND_MAX)*256);
    }
    
    std::auto_ptr<MemoryCard> card;
    try
    {
        card = std::auto_ptr<MemoryCard>(new MemoryCard(
            qpamat->set().readEntry("Smartcard/Library")) );
    }
    catch (const NoSuchLibraryException& e)
    {
        QApplication::restoreOverrideCursor();
        throw ReadWriteException(QObject::tr("The application was not set up correctly for "
            "using the smartcard. Call the configuration dialog and use the Test button for "
            "testing!<p>The error message was:<br><nobr>%1</nobr>").arg(e.what()), 
            ReadWriteException::CConfigurationError);
    }
    
    try
    {
        card->init(qpamat->set().readNumEntry("Smartcard/Port"));
    }
    catch (const CardException& e)
    {
        QApplication::restoreOverrideCursor();
        throw ReadWriteException(QObject::tr("Error in initializing the smart card reader:\n"
             "%1").arg(e.what()), ReadWriteException::CSmartcardError);
    }
    
    // ask the user to insert the smartcard
    QApplication::restoreOverrideCursor();
    
    QString pin;
    bool havePin = qpamat->set().readBoolEntry("Smartcard/HasWriteProtection") && write;
    std::auto_ptr<InsertCardDialog> dlg(new InsertCardDialog(havePin, m_parent, "InsertCardDlg"));
    if (dlg->exec() != QDialog::Accepted)
    {
        throw ReadWriteException(0, ReadWriteException::CAbort);
    }
    if (havePin)
    {
        pin = dlg->getPIN();
    }
    
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    
    // start the thread
    ReadWriteThread thread(*card, bytes, write, randomNumber, password, pin);
    thread.start();
    
    // show dialog
    QString dlgText = write 
        ? QObject::tr("<b>Writing</b> to the smartcard...") 
        : QObject::tr("<b>Reading</b> from the smartcard..."); 
    std::auto_ptr<WaitDialog> msg(new WaitDialog(QPixmap(
        QPixmap(":/images/smartcard_24.png")), dlgText, 
         "QPaMaT", m_parent, "Wait dialog"));
    msg->show();
    
    // loop while the thread has finished
    QTimer timer;
    timer.start(100, 0);
    while (thread.running())
    {
        qApp->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::WaitForMore);
    }
    timer.stop();
    
    // hide the dialog
    msg.reset();
    
    QApplication::restoreOverrideCursor();
    qApp->processEvents();
    
    // error handling
    ReadWriteException* ex = thread.getException();
    if (ex)
    {
        QApplication::restoreOverrideCursor();
        throw *ex;
    }
}


void DataReadWriter::crypt(QDomElement& n, StringEncryptor& enc, bool encrypt)
{
    QDomNodeList list = n.childNodes();
    if (list.length() != 0)
    {
        for (uint i = 0; i < list.length(); ++i)
        {
            QDomElement n = list.item(i).toElement();
            crypt(n, enc, encrypt);
        }
    }
    else
    {
        QDomElement el = n.toElement();
        if (!el.isNull())
        {
            if (el.tagName() == "property" && el.attribute("type") == "PASSWORD" 
                    && el.hasAttribute("value"))
            {
                if (encrypt)
                {
                    el.setAttribute("value", enc.encryptStrToStr(el.attribute("value")));    
                }
                else
                {
                    el.setAttribute("value", enc.decryptStrFromStr(el.attribute("value")));
                }
            }
        }
    }
}

// :maxLineLen=100:
