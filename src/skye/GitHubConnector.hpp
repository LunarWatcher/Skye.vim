#pragma once

#include "SiteAdapter.hpp"

namespace skye {

class GitHubConnector : public SiteAdapter {
private:
    static const inline std::string BASE_URL = "https://api.github.com";
    static const inline std::string ISSUE_URL = "/repos";
    static const inline std::string ISSUE_LIST = "/issues";
    static const inline std::string ISSUE_COMMENTS = "/comments";
        
    std::string repoPath;
    // cached issue list
    // We're not caching issue comments for the time being
    std::string issueList;

    void reparseUrl();

public:

    std::string getIssueAndComments(int forceRefresh) override;
    std::string getIssueList(int forceRefresh) override;

};

}
