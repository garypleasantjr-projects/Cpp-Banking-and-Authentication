#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "Account.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class AccountManager {
private:
    std::vector<Account> accounts;
    std::string databaseFile;

public:
    explicit AccountManager(const std::string& fileName);

    Account* findAccount(const std::string& username);
    bool createAccount(const std::string& username,
                       const std::string& password,
                       std::int64_t startingCents);

    // These operations save after each change, or undo it if saving fails.
    bool deposit(Account& account, std::int64_t cents);
    bool withdraw(Account& account, std::int64_t cents);
    bool changePassword(Account& account, const std::string& password);

    bool load();
    bool save() const;
    std::size_t accountCount() const;
}; // end class AccountManager

#endif
