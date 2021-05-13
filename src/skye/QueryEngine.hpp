#pragma once

#include "skye/SiteAdapter.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace skye {

class QueryEngine {
public:
    static std::shared_ptr<SiteAdapter> determineAdapterFromUrl(const std::string& site);
    static std::string parseUrl(const std::string& remoteName);

    static const char* queryIssue(const std::string& rawInput);
    static const char* queryIssueList(const std::string& rawInput);
};

} // namespace skye
