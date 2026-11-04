#include "AccountManager.h"
#include "Crypto.h"
#include "InputHandling.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void check(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    } // end if
} // end check()

int main() {
    const auto file = std::filesystem::temp_directory_path() / "intermediate_bank_test.db";
    const auto temp = file.string() + ".tmp";

    try {
        std::filesystem::remove(file);
        std::filesystem::remove(temp);

        // Input and money tests.
        check(InputHandling::validUsername("gary_1"), "Valid username");
        check(!InputHandling::validUsername("bad name"), "Invalid username");
        check(InputHandling::validPassword("a-long-example-password"), "Valid password");
        check(!InputHandling::validPassword("short"), "Short password");

        std::int64_t cents = 0;
        check(InputHandling::parseMoney("125.5", cents) && cents == 12550, "One-digit cents");
        check(InputHandling::parseMoney(".75", cents) && cents == 75, "No dollars part");
        check(InputHandling::parseMoney("10", cents) && cents == 1000, "Whole dollars");
        check(!InputHandling::parseMoney("12.345", cents), "Too many decimal places");
        check(!InputHandling::parseMoney("12.3.4", cents), "Two decimal points");
        check(!InputHandling::parseMoney("$10", cents), "Currency symbol");
        check(InputHandling::parseMoney("92233720368547758.07", cents), "Largest value");
        check(cents == INT64_MAX, "Largest cents value");
        check(!InputHandling::parseMoney("92233720368547758.08", cents), "Overflow rejected");
        check(cents == INT64_MAX, "Output unchanged on failure");

        // Crypto tests: same salt and password produce the same hash.
        const auto salt = Crypto::generateSalt();
        const auto hash1 = Crypto::hashPassword("a-long-example-password", salt);
        const auto hash2 = Crypto::hashPassword("a-long-example-password", salt);
        check(Crypto::sameHash(hash1, hash2), "Matching password hashes");
        check(!Crypto::sameHash(hash1, Crypto::hashPassword("another-long-password", salt)),
              "Different password hashes");
        check(Crypto::fromHex(Crypto::toHex(salt)) == salt, "Salt hex round-trip");

        // AccountManager tests: saving and loading actually work.
        {
            AccountManager manager(file.string());
            check(manager.load(), "New database starts empty");
            check(!manager.createAccount("bad name", "a-long-example-password", 0),
                  "Manager rejects invalid username");
            check(manager.createAccount("gary_1", "a-long-example-password", 10000),
                  "Create account");
            check(!manager.createAccount("gary_1", "a-long-example-password", 1),
                  "Duplicate username rejected");

            Account* account = manager.findAccount("gary_1");
            check(account != nullptr, "Find account");
            check(account->authenticate("a-long-example-password"), "Correct password");
            check(!account->authenticate("incorrect-password"), "Wrong password");
            check(manager.deposit(*account, 250), "Deposit");
            check(account->getBalanceCents() == 10250, "Deposit balance");
            check(!manager.withdraw(*account, 20000), "Insufficient funds");
            check(manager.withdraw(*account, 1000), "Withdrawal");
            check(account->getBalanceCents() == 9250, "Withdrawal balance");
            check(manager.changePassword(*account, "a-different-long-password"),
                  "Change password");
            check(!account->authenticate("a-long-example-password"), "Old password rejected");
        } // end manager scope

        {
            AccountManager reloaded(file.string());
            check(reloaded.load(), "Reload saved database");
            check(reloaded.accountCount() == 1, "One saved account");
            Account* account = reloaded.findAccount("gary_1");
            check(account != nullptr, "Loaded account exists");
            check(account->getBalanceCents() == 9250, "Balance survives reload");
            check(account->authenticate("a-different-long-password"),
                  "New password survives reload");
        } // end reloaded scope

        // Malformed database records must not silently load.
        {
            std::ofstream output(file);
            output << "BANK_DEMO_V1\n";
            output << "gary_1|ABCD|1234|100junk\n";
        } // end output scope
        AccountManager badManager(file.string());
        check(!badManager.load(), "Malformed database rejected");

        std::filesystem::remove(file);
        std::filesystem::remove(temp);
        std::cout << "All intermediate core tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::filesystem::remove(file);
        std::filesystem::remove(temp);
        std::cerr << "TEST FAILED: " << error.what() << '\n';
        return 1;
    } // end catch
} // end main()
