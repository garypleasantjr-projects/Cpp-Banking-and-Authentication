#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>

namespace Crypto {

// One place to set the password-hashing parameters.
constexpr int ITERATIONS = 600000;
constexpr int SALT_LENGTH = 16;
constexpr int HASH_LENGTH = 32;

std::vector<unsigned char> generateSalt();
std::vector<unsigned char> hashPassword(
    const std::string& password,
    const std::vector<unsigned char>& salt
);
bool sameHash(const std::vector<unsigned char>& a,
              const std::vector<unsigned char>& b);

// Store binary salt/hash bytes as readable hexadecimal text.
std::string toHex(const std::vector<unsigned char>& bytes);
std::vector<unsigned char> fromHex(const std::string& text);

} // end namespace Crypto

#endif
