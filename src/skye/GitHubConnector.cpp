#include "GitHubConnector.hpp"
#include "QueryEngine.hpp"
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

cpr::Header GitHubConnector::getHeaders(const std::string& token) {
    cpr::Header headers{{"User-Agent", "Skye.vim"}, {"Accept", "application/vnd.github.v3+json"}};

    if (token != "") {
        headers["Authorization"] = "token " + token;
    }
    return headers;
}

std::string GitHubConnector::getIssueAndComments(
        const std::string& url, const std::string& token, const std::string& issueId) {
    auto headers = getHeaders(token);

    auto repoPath = determineRepoPath(url);
    auto response =
            cpr::Get(cpr::Url{"https://api.github.com/repos" + repoPath + "/issues/" + issueId + "/comments"}, headers);

    auto rawIssueResponse =
            cpr::Get(cpr::Url{"https://api.github.com/repos" + repoPath + "/issues/" + issueId + ""}, headers);

    nlohmann::json issueObj = nlohmann::json::parse(rawIssueResponse.text);
    std::string ret;

    if (issueObj.is_object()) {
        if (issueObj.contains("message")) {
            return "Something unexpected happened: " + issueObj.at("message").get<std::string>();
        }
        nlohmann::json commentObj = nlohmann::json::parse(response.text);
        ret += "# #" + std::to_string(issueObj.at("number").get<int>()) + ": " //
               + issueObj.at("title").get<std::string>() + "\n";
        ret += "OP: " + issueObj.at("user").at("login").get<std::string>() + "\n";
        ret += "URL: " + issueObj.at("html_url").get<std::string>() + "\n";
        ret += "State: " + issueObj.at("state").get<std::string>() + "\n\n";
        {
            auto issueBody = issueObj.at("body").get<std::string>();
            String::purgeBadNewlineCharacter(issueBody);
            ret += issueBody;
        }

        for (auto& issue : commentObj) {
            ret += "\n\n------------------------------------------------\n\n";
            ret += "Comment by " + issue.at("user").at("login").get<std::string>() + "\n";
            ret += "URL: " + issue.at("html_url").get<std::string>() + "\n\n";

            auto commentBody = issue.at("body").get<std::string>();
            String::purgeBadNewlineCharacter(commentBody);
            ret += commentBody;
        }
    } else {
        return "The API returned an unexpected object: " + response.text;
    }
    return ret;
}

std::string GitHubConnector::getIssueList(
        const std::string& url, const std::string& token, const std::string& apiParameters) {
    auto headers = getHeaders(token);

    auto response = cpr::Get(
            cpr::Url{"https://api.github.com/repos" + determineRepoPath(url) + "/issues" + apiParameters}, headers);

    auto remainingQuota = response.header["x-ratelimit-remaining"];
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
        if (remainingQuota == "0") {
            return "Out of quota.\n\nSee `:h skye-github` for more information on rate limiting and tokens";
        }
        return "Something unexpected happened. Message from the API: " + obj["message"].get<std::string>();
    } else {
        return "The API returned an unexpected object: " + response.text;
    }

    return ret;
}

} // namespace skye
