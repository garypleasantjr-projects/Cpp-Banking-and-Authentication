#include "Account.h"
#include "Crypto.h"

#include <limits>
#include <stdexcept>

Account::Account(const std::string& name, const std::string& password,
                 std::int64_t startingCents)
    : username(name), salt(Crypto::generateSalt()),
      passwordHash(Crypto::hashPassword(password, salt)),
      balanceCents(startingCents) {
    if (startingCents < 0) {
        throw std::invalid_argument("Balance cannot be negative.");
    } // end if
} // end Account() for new accounts

Account::Account(const std::string& name,
                 const std::vector<unsigned char>& savedSalt,
                 const std::vector<unsigned char>& savedHash,
                 std::int64_t savedCents)
    : username(name), salt(savedSalt), passwordHash(savedHash),
      balanceCents(savedCents) {
    if (savedCents < 0 || salt.size() != Crypto::SALT_LENGTH ||
        passwordHash.size() != Crypto::HASH_LENGTH) {
        throw std::invalid_argument("Invalid saved account.");
    } // end if
} // end Account() for loaded accounts

const std::string& Account::getUsername() const {
    return username;
} // end getUsername()

const std::vector<unsigned char>& Account::getSalt() const {
    return salt;
} // end getSalt()

const std::vector<unsigned char>& Account::getPasswordHash() const {
    return passwordHash;
} // end getPasswordHash()

std::int64_t Account::getBalanceCents() const {
    return balanceCents;
} // end getBalanceCents()

bool Account::authenticate(const std::string& password) const {
    const auto attemptedHash = Crypto::hashPassword(password, salt);
    return Crypto::sameHash(attemptedHash, passwordHash);
} // end authenticate()

void Account::changePassword(const std::string& newPassword) {
    // Finish hashing before replacing the old salt and hash.
    const auto newSalt = Crypto::generateSalt();
    const auto newHash = Crypto::hashPassword(newPassword, newSalt);
    salt = newSalt;
    passwordHash = newHash;
} // end changePassword()

bool Account::deposit(std::int64_t cents) {
    // Check BEFORE adding to avoid overflowing a 64-bit integer.
    if (cents <= 0 ||
        cents > std::numeric_limits<std::int64_t>::max() - balanceCents) {
        return false;
    } // end if
    balanceCents += cents;
    return true;
} // end deposit()

bool Account::withdraw(std::int64_t cents) {
    if (cents <= 0 || cents > balanceCents) {
        return false;
    } // end if
    balanceCents -= cents;
    return true;
} // end withdraw()
