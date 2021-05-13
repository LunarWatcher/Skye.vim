#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace skye {

namespace String {

inline std::vector<std::string> split(const std::string& input, char splitAt) {
    std::stringstream ss(input);
    std::string currToken;
    std::vector<std::string> retVal;
    while (getline(ss, currToken, splitAt)) {
        retVal.push_back(currToken);
    }
    return retVal;
}

inline std::string capString(const std::string& in, size_t maxLen) {
    if (in.size() < maxLen)
        return in;
    return in.substr(0, maxLen) + " ...";
}

inline void purgeBadNewlineCharacter(std::string& input) {
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
}

} // namespace String

} // namespace skye
