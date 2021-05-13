#include "QueryEngine.hpp"
#include "GitHubConnector.hpp"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "util/Strings.hpp"

#include <iostream>

namespace skye {

std::string GitHubConnector::determineRepoPath(const std::string& rawUrl) {
    // assumes non-weird paths. Shouldn't be a problem
    // https:// is len 8
    auto url = rawUrl.substr(rawUrl.find('/', 8));
    return url;
}

std::string GitHubConnector::getIssueAndComments(const std::string& , const std::string ) {
    return "hi";
}

std::string GitHubConnector::getIssueList(const std::string& url, const std::string token) {

    // GH shouldn't reject this UA
    cpr::Header headers{{"User-Agent", "Skye"}};
    if (token != "") {
        headers["Authorization"] = "token " + token;
    }

    // TODO: support state search
    auto response = cpr::Get(cpr::Url{"https://api.github.com/repos" + determineRepoPath(url) + "/issues"});
    // TODO: Figure out quota management and backoffs. No storage in C++ means this has to be returned somehow
    auto remainingQuota = response.header["x-ratelimit-remaining"];

    std::string ret;

    nlohmann::json obj = nlohmann::json::parse(response.text);
    for (auto& issue : obj) {
        auto url = issue.at("html_url").get<std::string>();
        auto title = issue.at("title").get<std::string>();
        auto number = std::to_string(issue.at("number").get<int>());
        
        // TODO: check if body is empty or null if missing
        auto rawBody = issue.at("body").get<std::string>();
        String::purgeBadNewlineCharacter(rawBody);
        auto body = String::capString(rawBody, 300);
        auto user = issue.at("user").at("login").get<std::string>();
        ret += "# #" + number + ": " + title + "\n";
        ret += "Posted by " + user + " at " + url + "\n";
        ret += "\n" + body + "\n\n";
    }
    
    return ret;
}


}
