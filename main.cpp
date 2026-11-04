#include "AccountManager.h"
#include "InputHandling.h"

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

// The main program only handles menus; other files do the banking work.
void createAccountMenu(AccountManager& manager) {
    const std::string username = InputHandling::readLine("New username: ");
    const std::string password = InputHandling::readLine("New password: ");
    const std::int64_t startingCents =
        InputHandling::readMoney("Starting balance: ", true);

    if (manager.createAccount(username, password, startingCents)) {
        std::cout << "Account created.\n";
    } else {
        std::cout << "Could not create account. Check username, password, or duplicate.\n";
    } // end if
} // end createAccountMenu()

void loggedInMenu(AccountManager& manager, Account& account) {
    while (true) {
        std::cout << "\nLogged in as " << account.getUsername() << '\n'
                  << "1. View balance\n"
                  << "2. Deposit\n"
                  << "3. Withdraw\n"
                  << "4. Change password\n"
                  << "5. Log out\n";
        const int choice = InputHandling::readChoice("Choice: ", 1, 5);

        if (choice == 1) {
            std::cout << "Balance: "
                      << InputHandling::formatMoney(account.getBalanceCents()) << '\n';
        } else if (choice == 2) {
            const auto cents = InputHandling::readMoney("Deposit amount: ");
            std::cout << (manager.deposit(account, cents) ? "Deposit saved.\n" : "Deposit failed.\n");
        } else if (choice == 3) {
            const auto cents = InputHandling::readMoney("Withdrawal amount: ");
            std::cout << (manager.withdraw(account, cents) ? "Withdrawal saved.\n" : "Withdrawal failed.\n");
        } else if (choice == 4) {
            const std::string oldPassword = InputHandling::readLine("Current password: ");
            if (!account.authenticate(oldPassword)) {
                std::cout << "Incorrect password.\n";
                continue;
            } // end if
            const std::string newPassword = InputHandling::readLine("New password: ");
            std::cout << (manager.changePassword(account, newPassword)
                          ? "Password changed.\n" : "Password change failed.\n");
        } else {
            return;
        } // end else
    } // end while
} // end loggedInMenu()

void loginMenu(AccountManager& manager) {
    const std::string username = InputHandling::readLine("Username: ");
    const std::string password = InputHandling::readLine("Password: ");
    Account* account = manager.findAccount(username);
    if (account == nullptr || !account->authenticate(password)) {
        std::cout << "Incorrect username or password.\n";
        return;
    } // end if
    loggedInMenu(manager, *account);
} // end loginMenu()

int main() {
    try {
        std::filesystem::create_directories("data");
        AccountManager manager("data/accounts.db");
        if (!manager.load()) {
            std::cerr << "Could not safely open the saved accounts database.\n";
            return 1;
        } // end if

        std::cout << "Secure C++ Banking Demo (educational only)\n"
                  << "Loaded accounts: " << manager.accountCount() << '\n';
        while (true) {
            std::cout << "\n1. Create account\n2. Log in\n3. Exit\n";
            const int choice = InputHandling::readChoice("Choice: ", 1, 3);
            if (choice == 1) {
                createAccountMenu(manager);
            } else if (choice == 2) {
                loginMenu(manager);
            } else {
                std::cout << "Goodbye.\n";
                break;
            } // end else
        } // end while
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    } // end catch
    return 0;
} // end main()
