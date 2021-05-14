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

inline std::string capString(const std::string& in, size_t maxLen) {
    if (in.size() < maxLen)
        return in;
    std::string result = in.substr(0, maxLen) + " ...";
    // To avoid format leaking
    // This doesn't account for other bits of formatting, but what can you do?
    if (countSubstring(result, "<!--") > countSubstring(result, "-->")) {
        result += "-->\n";
    }
    if (countSubstring(result, "```") % 2 != 0) {
        result += "\n```\n";
    }

    return result;
}

inline void purgeBadNewlineCharacter(std::string& input) {
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
}

} // namespace String

} // namespace skye
