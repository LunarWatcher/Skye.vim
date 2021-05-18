#pragma once

#include "SiteAdapter.hpp"
#include <iostream>
#include "cpr/cpr.h"

namespace skye {

class GitHubConnector : public SiteAdapter {
public:
    static std::string determineRepoPath(const std::string& rawUrl);
    static cpr::Header getHeaders(const std::string& token);

    std::string getIssueAndComments(const std::string& url, const std::string& token, const std::string& issueId) override;
    std::string getIssueList(const std::string& url, const std::string& token, const std::string& apiParameters) override;

};

}
