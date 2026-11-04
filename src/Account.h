#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <cstdint>
#include <string>
#include <vector>

class Account {
private:
    std::string username;
    std::vector<unsigned char> salt;
    std::vector<unsigned char> passwordHash;
    std::int64_t balanceCents;

public:
    // For creating a new account from a password.
    Account(const std::string& name, const std::string& password,
            std::int64_t startingCents);

    // For loading an account with an existing saved salt and hash.
    Account(const std::string& name,
            const std::vector<unsigned char>& savedSalt,
            const std::vector<unsigned char>& savedHash,
            std::int64_t savedCents);

    const std::string& getUsername() const;
    const std::vector<unsigned char>& getSalt() const;
    const std::vector<unsigned char>& getPasswordHash() const;
    std::int64_t getBalanceCents() const;

    bool authenticate(const std::string& password) const;
    void changePassword(const std::string& newPassword);
    bool deposit(std::int64_t cents);
    bool withdraw(std::int64_t cents);
}; // end class Account

#endif
