#pragma once

#include "SiteAdapter.hpp"
#include <iostream>

namespace skye {

class GitHubConnector : public SiteAdapter {
private:
    static inline std::string token = "";

    std::string repoPath;
    // cached issue list
    // We're not caching issue comments for the time being
    std::string issueList;

    void reparseUrl();

public:

    std::string getIssueAndComments(int forceRefresh) override;
    std::string getIssueList(int forceRefresh) override;

    static void setToken(const char* token) {
        GitHubConnector::token = token;
    }

};

}
