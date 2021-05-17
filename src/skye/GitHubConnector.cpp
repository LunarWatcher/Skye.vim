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

std::string GitHubConnector::getIssueAndComments(const std::string& url, const std::string& token, const std::string& issueId) {
    cpr::Header headers{
        {"User-Agent", "Skye.vim"},
        {"Accept", "application/vnd.github.v3+json"}
    };

    if (token != "") {
        headers["Authorization"] = "token " + token;
    }

    auto repoPath = determineRepoPath(url);
    auto response = cpr::Get(cpr::Url{"https://api.github.com/repos" + repoPath + "/issues/" + issueId + "/comments"}, headers);

    auto rawIssueResponse = cpr::Get(cpr::Url{"https://api.github.com/repos" + repoPath + "/issues/" + issueId + "/comments"}, headers);

    nlohmann::json obj = nlohmann::json::parse(response.text);
    std::string ret;

    if (obj.is_array()) {
        for (auto& issue : obj) {
            auto url = issue.at("html_url").get<std::string>();
            auto title = issue.at("title").get<std::string>();
            auto number = std::to_string(issue.at("number").get<int>());
            
            // TODO: check if body is empty or null if missing
            auto rawBody = issue.at("body").get<std::string>();
            auto user = issue.at("user").at("login").get<std::string>();

            auto state = "**" + issue.at("state").get<std::string>() + "**";

            // markdown header
            ret += "# #" + number + ": " + title + "\n";
            ret += "State: " + state + "\n";
            ret += "Posted by " + user + " at " + url + "\n";
        }
    } else if (obj.is_object()) {
        auto remainingQuota = std::stoi(response.header["x-ratelimit-remaining"]);
        if (remainingQuota == 0) {
            return "Sorry, out of quota.\n\nSee `:h skye-github` for more information on rate limiting and tokens";
        }
        return "Something unexpected happened. Message from the API: " + obj["message"].get<std::string>();
    } else {
        return "The API returned an unexpected object: " + response.text;
    }
    return ret;
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
    auto response = cpr::Get(cpr::Url{"https://api.github.com/repos" + determineRepoPath(url) + "/issues" + apiParameters}, headers);
    // TODO: Figure out quota management and backoffs. No storage in C++ means this has to be returned somehow
    auto remainingQuota = response.header["x-ratelimit-remaining"];
    if (remainingQuota == "0") {
        return "# Sorry, out of quota.\n\nSee `:h skye-github` for more information on rate limiting and tokens";
    }


    std::string ret = "Remaining quota: " + remainingQuota + " (max: " + response.header["x-ratelimit-limit"] + ")\n\n";

    nlohmann::json obj = nlohmann::json::parse(response.text);
    if (obj.is_array()) {
        for (auto& issue : obj) {
            auto url = issue.at("html_url").get<std::string>();
            auto title = issue.at("title").get<std::string>();
            auto number = std::to_string(issue.at("number").get<int>());

            auto user = issue.at("user").at("login").get<std::string>();

            auto state = issue.at("state").get<std::string>() == "closed" ? "c" : "o";

            // markdown header
            ret += "#" + number + "\t[" + state + "]\t" + title + " by " + user + " at " + url + "\n";

        }
    } else if (obj.is_object()) {
        return "Something unexpected happened. Message from the API: " + obj["message"].get<std::string>();
    } else {
        return "The API returned an unexpected object: " + response.text;
    }
    
    return ret;
}


}
