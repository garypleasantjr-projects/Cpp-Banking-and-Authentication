#include "AccountManager.h"
#include "Crypto.h"
#include "InputHandling.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>

AccountManager::AccountManager(const std::string& fileName)
    : databaseFile(fileName) {} // end AccountManager() constructor

Account* AccountManager::findAccount(const std::string& username) {
    for (Account& account : accounts) {
        if (account.getUsername() == username) {
            return &account;
        } // end if
    } // end for
    return nullptr;
} // end findAccount()

bool AccountManager::createAccount(const std::string& username,
                                   const std::string& password,
                                   std::int64_t startingCents) {
    if (!InputHandling::validUsername(username) ||
        !InputHandling::validPassword(password) ||
        startingCents < 0 || findAccount(username) != nullptr) {
        return false;
    } // end if

    accounts.emplace_back(username, password, startingCents);
    if (!save()) {
        accounts.pop_back(); // Undo if the database could not be saved.
        return false;
    } // end if
    return true;
} // end createAccount()

bool AccountManager::deposit(Account& account, std::int64_t cents) {
    if (!account.deposit(cents)) {
        return false;
    } // end if
    if (!save()) {
        account.withdraw(cents); // Undo the deposit.
        return false;
    } // end if
    return true;
} // end deposit()

bool AccountManager::withdraw(Account& account, std::int64_t cents) {
    if (!account.withdraw(cents)) {
        return false;
    } // end if
    if (!save()) {
        account.deposit(cents); // Undo the withdrawal.
        return false;
    } // end if
    return true;
} // end withdraw()

bool AccountManager::changePassword(Account& account, const std::string& password) {
    if (!InputHandling::validPassword(password)) {
        return false;
    } // end if
    const Account previous = account; // Backup for a failed save.
    account.changePassword(password);
    if (!save()) {
        account = previous;
        return false;
    } // end if
    return true;
} // end changePassword()

bool AccountManager::load() {
    // Only treat a genuinely missing file as a new empty database.
    std::error_code error;
    const bool exists = std::filesystem::exists(databaseFile, error);
    if (error) {
        return false;
    } // end if
    if (!exists) {
        return true;
    } // end if

    std::ifstream input(databaseFile);
    if (!input) {
        return false;
    } // end if

    std::string header;
    if (!std::getline(input, header) || header != "BANK_DEMO_V1") {
        return false;
    } // end if

    // Load to a temporary vector so invalid data cannot partially load.
    std::vector<Account> loaded;
    std::string line;
    try {
        while (std::getline(input, line)) {
            if (line.empty() || line.back() == '|') {
                return false;
            } // end if

            std::stringstream stream(line);
            std::string username, saltText, hashText, balanceText, extra;
            if (!std::getline(stream, username, '|') ||
                !std::getline(stream, saltText, '|') ||
                !std::getline(stream, hashText, '|') ||
                !std::getline(stream, balanceText, '|') ||
                std::getline(stream, extra, '|')) {
                return false;
            } // end if

            if (!InputHandling::validUsername(username)) {
                return false;
            } // end if
            for (const Account& other : loaded) {
                if (other.getUsername() == username) {
                    return false;
                } // end if
            } // end for

            // Every character in the saved balance must be a digit.
            for (unsigned char character : balanceText) {
                if (!std::isdigit(character)) {
                    return false;
                } // end if
            } // end for
            std::size_t used = 0;
            const std::int64_t cents = std::stoll(balanceText, &used);
            if (used != balanceText.size() || cents < 0) {
                return false;
            } // end if
            loaded.emplace_back(username, Crypto::fromHex(saltText),
                                Crypto::fromHex(hashText), cents);
        } // end while
    } catch (const std::exception&) {
        return false;
    } // end catch

    if (input.bad()) {
        return false;
    } // end if
    accounts = std::move(loaded);
    return true;
} // end load()

bool AccountManager::save() const {
    const std::string tempFile = databaseFile + ".tmp";
    std::ofstream output(tempFile);
    if (!output) {
        return false;
    } // end if

    output << "BANK_DEMO_V1\n";
    for (const Account& account : accounts) {
        output << account.getUsername() << '|'
               << Crypto::toHex(account.getSalt()) << '|'
               << Crypto::toHex(account.getPasswordHash()) << '|'
               << account.getBalanceCents() << '\n';
    } // end for
    output.close();
    if (!output) {
        std::error_code ignored;
        std::filesystem::remove(tempFile, ignored);
        return false;
    } // end if

    // Replace the old file only after writing the complete new one.
    std::error_code error;
    std::filesystem::rename(tempFile, databaseFile, error);
    if (error) {
        std::error_code ignored;
        std::filesystem::remove(tempFile, ignored);
        return false;
    } // end if
    return true;
} // end save()

std::size_t AccountManager::accountCount() const {
    return accounts.size();
} // end accountCount()
