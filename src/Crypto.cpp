#include "Crypto.h"

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace Crypto {

std::vector<unsigned char> generateSalt() {
    std::vector<unsigned char> salt(SALT_LENGTH);
    if (RAND_bytes(salt.data(), SALT_LENGTH) != 1) {
        throw std::runtime_error("Could not generate a secure salt.");
    } // end if
    return salt;
} // end generateSalt()

std::vector<unsigned char> hashPassword(
    const std::string& password,
    const std::vector<unsigned char>& salt
) {
    if (salt.size() != SALT_LENGTH) {
        throw std::invalid_argument("Wrong salt size.");
    } // end if

    std::vector<unsigned char> hash(HASH_LENGTH);
    const int success = PKCS5_PBKDF2_HMAC(
        password.data(), static_cast<int>(password.size()),
        salt.data(), static_cast<int>(salt.size()),
        ITERATIONS, EVP_sha256(), HASH_LENGTH, hash.data()
    );
    if (success != 1) {
        throw std::runtime_error("Password hashing failed.");
    } // end if
    return hash;
} // end hashPassword()

bool sameHash(const std::vector<unsigned char>& a,
              const std::vector<unsigned char>& b) {
    if (a.size() != HASH_LENGTH || b.size() != HASH_LENGTH) {
        return false;
    } // end if
    return CRYPTO_memcmp(a.data(), b.data(), a.size()) == 0;
} // end sameHash()

std::string toHex(const std::vector<unsigned char>& bytes) {
    std::ostringstream output;
    output << std::hex << std::setfill('0');
    for (unsigned char byte : bytes) {
        output << std::setw(2) << static_cast<int>(byte);
    } // end for
    return output.str();
} // end toHex()

std::vector<unsigned char> fromHex(const std::string& text) {
    if (text.size() % 2 != 0) {
        throw std::invalid_argument("Hex text must have an even length.");
    } // end if

    for (unsigned char character : text) {
        if (!std::isxdigit(character)) {
            throw std::invalid_argument("Invalid hex character.");
        } // end if
    } // end for

    std::vector<unsigned char> bytes;
    for (std::size_t i = 0; i < text.size(); i += 2) {
        const std::string pair = text.substr(i, 2);
        std::size_t used = 0;
        const unsigned long value = std::stoul(pair, &used, 16);
        if (used != 2 || value > 255) {
            throw std::invalid_argument("Invalid hex text.");
        } // end if
        bytes.push_back(static_cast<unsigned char>(value));
    } // end for
    return bytes;
} // end fromHex()

} // end namespace Crypto
