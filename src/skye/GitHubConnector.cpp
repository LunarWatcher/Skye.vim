#include "QueryEngine.hpp"
#include "GitHubConnector.hpp"
#include "cpr/cpr.h"

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
    return "1. Blah\n2. Blah";
}


}
