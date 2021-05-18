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

inline size_t countSubstring(const std::string& in, const std::string& search) {
    size_t occurences = 0;
    size_t offset = 0;

    while ((offset = in.find(search, offset)) != std::string::npos) {
        ++occurences;
        offset += search.size();
    }

    return occurences;
}

inline void purgeBadNewlineCharacter(std::string& input) {
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
}

} // namespace String

} // namespace skye
