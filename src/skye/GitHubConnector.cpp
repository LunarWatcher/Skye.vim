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

std::string GitHubConnector::getIssueAndComments(const std::string& , const std::string&, const std::string&) {
    return "hi";
}

std::string GitHubConnector::getIssueList(const std::string& url, const std::string& token, const std::string& apiParameters) {

    // GH shouldn't reject this UA
    cpr::Header headers{
        {"User-Agent", "Skye.vim"},
        {"Accept", "application/vnd.github.v3+json"}
    };

    if (token != "") {
        headers["Authorization"] = "token " + token;
    }

    // TODO: support state search
    auto response = cpr::Get(cpr::Url{"https://api.github.com/repos" + determineRepoPath(url) + "/issues" + apiParameters});
    // TODO: Figure out quota management and backoffs. No storage in C++ means this has to be returned somehow
    auto remainingQuota = std::stoi(response.header["x-ratelimit-remaining"]);
    if (remainingQuota == 0) {
        return "# Sorry, out of quota.\n\nSee `:h skye-github` for more information on rate limiting and tokens";
    }

    // TODO: include total quota? x-ratelimit-total or whatever (check with cURL)
    std::string ret = "Remaining quota: " + std::to_string(remainingQuota) + "\n\n";
    // TODO: error handling

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

        auto state = "**" + issue.at("state").get<std::string>() + "**";

        // markdown header
        ret += "# #" + number + ": " + title + "\n";
        ret += "State: " + state + "\n";
        ret += "Posted by " + user + " at " + url + "\n";
        ret += "\n" + body + "\n\n";
    }
    
    return ret;
}


}
