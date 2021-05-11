#include "QueryEngine.hpp"
#include "GitHubConnector.hpp"
#include "cpr/cpr.h"

#include <iostream>

namespace skye {

void GitHubConnector::reparseUrl() {
    auto& rawUrl = QueryEngine::getInstance()->getUrl();
    auto url = rawUrl.substr(rawUrl.find('/'));

    this->repoPath = url;
}

std::string GitHubConnector::getIssueAndComments(int forceRefresh) {
    return "hi";
}

std::string GitHubConnector::getIssueList(int forceRefresh) {
    // TODO: figure out more sane caching rules
    if (!forceRefresh && this->issueList != "") {
        return this->issueList;
    }

    cpr::Header headers{{"User-Agent", "Skye"}};
    if (GitHubConnector::token != "") {
        headers["Authorization"] = "token " + this->token;
        std::cout << "Authorized" << std::endl;
    }

    // TODO: support state search
    auto response = cpr::Get(cpr::Url{"https://api.github.com/repos" + repoPath + "/issues"});
    std::cout << response.header["x-ratelimit-remaining"] << std::endl;
    
    return "hi";
}


}
