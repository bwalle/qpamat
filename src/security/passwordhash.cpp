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
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <iterator>

#include <QDebug>
#include <QString>
#include <Q3CString>

#include <openssl/evp.h>

#if OPENSSL_VERSION_NUMBER < 0x10100000
#define EVP_MD_CTX_new EVP_MD_CTX_create
#define EVP_MD_CTX_free EVP_MD_CTX_destroy
#endif

#include "global.h"
#include "constants.h"
#include "passwordhash.h"
#include "encodinghelper.h"
#include "util/stdrandomnumbergenerator.h"

// -------------------------------------------------------------------------------------------------
//                                     Static data
// -------------------------------------------------------------------------------------------------

const int PasswordHash::numberOfRandomBytes = 8;

/**
 * This constant describes the maximal length of the hash.
 *
 * The current implementation uses 8 byte of salt and 20 bytes (160 bit) of hash data,
 * that makes 28 bytes. Since SHA1 has the same length, a change from RIPE-MD160 to SHA1
 * doesn't affect the length.
 *
 * This value is used for the alignment on the chipcard. We need fixed alignments, so a
 * change of this constant makes old chipcards unusable. That is the reason why I don't
 * use the constant \c EVP_MAX_MD_SIZE here. I use an assertion to check that the actual
 * value has a length smaller or equal to this constant.
 */
const int PasswordHash::MAX_HASH_LENGTH = 40;

/**
 * @class PasswordHash
 *
 * @brief Helping functions for dealing with passwords and hashes.
 * @ingroup security
 * @author Bernhard Walle
 */

/**
 * Checks the given password.
 *
 * Calls the PasswordHash::isCorrect(QString, const ByteVector&) method but decodes the
 * string with base64 previously.
 *
 * @param password the password
 * @param hash the hash object
 *
 * @return \c true if the password is correct, \c false otherwise
 */
bool PasswordHash::isCorrect(QString password, const QString& hash)
{
    bool correct = false;
    try {
        correct = isCorrect(password, EncodingHelper::fromBase64(hash));
    } catch (const std::invalid_argument& e) {
        qDebug() << CURRENT_FUNCTION << "Caught invalid_argument:" << e.what();
    }
    return correct;
}


/**
 * @brief Checks the given password if it is the same as the one stored in the hash.
 *
 * (There's no way to re-construct the password from the hash. There's a hash generated
 * from the password and this one is compared to that which was provided by \p hash.)
 *
 * @param password the password
 * @param hash the hash object
 * @return \c true if the password is correct, \c false otherwise
 */
bool PasswordHash::isCorrect(QString password, const ByteVector& hash)
{
    ByteVector output;

    Q_ASSERT(hash.size() > numberOfRandomBytes);

    // attach the random bytes
    ByteVector passwordBytes(numberOfRandomBytes);
    std::copy(hash.begin(), hash.begin() + numberOfRandomBytes, passwordBytes.begin());

    // convert the password to a byte vector
    QByteArray passwordCString = password.toUtf8();
    std::copy(passwordCString.begin(), passwordCString.end(), std::back_inserter(passwordBytes));

    attachHashWithoutSalt(output, passwordBytes);

    return std::equal(output.begin(), output.end(), hash.begin() + numberOfRandomBytes);
}


/**
 * Generates a hash from the password.
 *
 * That hash contains a salt, too, which makes storing and veryfiing hashes more secure.
 * To be more precisely, a so-called dictinoary attack is prepended which this method
 * because the dictionary would be very large.
 */
ByteVector PasswordHash::generateHash(QString password)
{
    StdRandomNumberGenerator<unsigned char> rand;
    ByteVector output(numberOfRandomBytes);

    // generate the random bytes and copy them to the output, too
    ByteVector passwordBytes(numberOfRandomBytes);
    std::generate(output.begin(), output.end(), rand);
    std::copy(output.begin(), output.end(), passwordBytes.begin());

    // attach the password
    QByteArray passwordCString = password.toUtf8();
    std::copy(passwordCString.begin(), passwordCString.end(), std::back_inserter(passwordBytes));

    attachHashWithoutSalt(output, passwordBytes);

    Q_ASSERT(output.size() <= MAX_HASH_LENGTH);

    return output;
}


/**
 * @brief Generates a hash from the password.
 *
 * Calls the PasswordHash::generateHash() method and encodes the bytes with base 64.
 *
 * @param password the password to hash
 */
QString PasswordHash::generateHashString(const QString& password)
{
    return EncodingHelper::toBase64(generateHash(password));
}


/**
 * @brief Attaches hash without salt
 *
 * @param output the output
 * @param passwordBytes the password including the "salt" attached
 * @todo Find out what the function really does
 */
void PasswordHash::attachHashWithoutSalt(ByteVector& output, const ByteVector& passwordBytes)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    int len = passwordBytes.size();
    unsigned char* password = new unsigned char[len];
    std::copy(passwordBytes.begin(), passwordBytes.end(), password);

    EVP_DigestInit(mdctx, HASH_ALGORITHM);
    EVP_DigestUpdate(mdctx, password, len);
    EVP_DigestFinal(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    delete[] password;

    std::copy(md_value, md_value + md_len, std::back_inserter(output));
}

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:
