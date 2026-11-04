#ifndef INPUT_HANDLING_H
#define INPUT_HANDLING_H

#include <cstdint>
#include <string>

namespace InputHandling {

std::string readLine(const std::string& prompt);
int readChoice(const std::string& prompt, int min, int max);
bool validUsername(const std::string& name);
bool validPassword(const std::string& password);
bool parseMoney(const std::string& text, std::int64_t& centsOut);
std::int64_t readMoney(const std::string& prompt, bool allowZero = false);
std::string formatMoney(std::int64_t cents);

} // end namespace InputHandling

#endif
