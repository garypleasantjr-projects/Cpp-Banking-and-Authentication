#include "InputHandling.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace InputHandling {

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("Input ended.");
    } // end if
    return line;
} // end readLine()

int readChoice(const std::string& prompt, int min, int max) {
    while (true) {
        const std::string text = readLine(prompt);
        try {
            std::size_t used = 0;
            const int value = std::stoi(text, &used);
            if (used == text.size() && value >= min && value <= max) {
                return value;
            } // end if
        } catch (const std::exception&) {
            // Invalid text: print an error and ask again.
        } // end catch
        std::cout << "Enter a number from " << min << " to " << max << ".\n";
    } // end while
} // end readChoice()

bool validUsername(const std::string& name) {
    if (name.size() < 3 || name.size() > 16) {
        return false;
    } // end if
    for (unsigned char character : name) {
        if (!std::isalnum(character) && character != '_') {
            return false;
        } // end if
    } // end for
    return true;
} // end validUsername()

bool validPassword(const std::string& password) {
    if (password.size() < 12 || password.size() > 128) {
        return false;
    } // end if
    for (unsigned char character : password) {
        if (std::iscntrl(character)) {
            return false;
        } // end if
    } // end for
    return true;
} // end validPassword()

bool parseMoney(const std::string& text, std::int64_t& centsOut) {
    if (text.empty() || text == ".") {
        return false;
    } // end if

    // Split "125.5" into "125" and "5".
    const std::size_t dot = text.find('.');
    if (dot != std::string::npos &&
        text.find('.', dot + 1) != std::string::npos) {
        return false; // Reject a second decimal point.
    } // end if
    for (unsigned char character : text) {
        if (character != '.' && !std::isdigit(character)) {
            return false;
        } // end if
    } // end for

    std::string dollarsPart = text.substr(0, dot);
    std::string centsPart = dot == std::string::npos ? "" : text.substr(dot + 1);
    if (dollarsPart.empty()) {
        dollarsPart = "0";
    } // end if
    if (centsPart.size() > 2) {
        return false;
    } // end if
    if (centsPart.size() == 1) {
        centsPart += '0'; // "5" means 50 cents, not 5 cents.
    } // end if

    try {
        const std::int64_t dollars = std::stoll(dollarsPart);
        const std::int64_t cents = centsPart.empty() ? 0 : std::stoll(centsPart);
        if (dollars > (std::numeric_limits<std::int64_t>::max() - cents) / 100) {
            return false;
        } // end if
        centsOut = dollars * 100 + cents;
        return true;
    } catch (const std::exception&) {
        return false;
    } // end catch
} // end parseMoney()

std::int64_t readMoney(const std::string& prompt, bool allowZero) {
    while (true) {
        std::int64_t cents = 0;
        if (parseMoney(readLine(prompt), cents) && (cents > 0 || allowZero)) {
            return cents;
        } // end if
        std::cout << "Enter a valid amount such as 12.50.\n";
    } // end while
} // end readMoney()

std::string formatMoney(std::int64_t cents) {
    std::ostringstream output;
    output << '$' << (cents / 100) << '.'
           << std::setw(2) << std::setfill('0') << (cents % 100);
    return output.str();
} // end formatMoney()

} // end namespace InputHandling
