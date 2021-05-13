#include "QueryEngine.hpp"
#include "GitHubConnector.hpp"
#include "util/Strings.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string.h>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace skye {

std::string QueryEngine::parseUrl(const std::string& remoteName) {
    std::array<char, 128> buff;
    std::string res;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("git remote -v", "r"), pclose);
    if (!pipe) {
        return "";
    }
    while (fgets(buff.data(), buff.size(), pipe.get()) != nullptr) {
        res += buff.data();
    }
    // And convert that trash ^ to something useful
    std::stringstream stream(res);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.starts_with(remoteName)) {
            std::string remote = line.substr(line.find('\t') + 1);
            remote = remote.substr(0, remote.rfind('(') - 1);
            if (remote.starts_with("git@")) {
                remote = remote.substr(remote.find('@') + 1);
                remote = "https://" + remote.replace(remote.find(':'), 1, "/");
            }

            if (remote.ends_with(".git")) {
                remote = remote.substr(0, remote.rfind('.'));
            }
            return remote;
        }
    }
    return "";
}

const char* QueryEngine::queryIssue(const std::string& rawInput) {
    auto components = String::split(rawInput, ';');
    auto url = components[0];
    auto token = components[1];
    // This is, contextually, a number in some places.
    // In other systems, however, it's a string.
    // Also, it makes no practical difference. It's shoved
    // into the URL, so converting from a string to a number
    // would require turning this back into a string.
    //
    // The API deals with error handling
    auto issue = components[2];

    auto adapter = determineAdapterFromUrl(url);
    if (adapter == nullptr) {
        return strcat(strdup("Failed to determine adapter from url "), url.c_str());
    }

    return strdup(adapter->getIssueAndComments(url, token, issue).c_str());
}

const char* QueryEngine::queryIssueList(const std::string& rawInput) {
    auto components = String::split(rawInput, ';');
    auto url = components[0];
    auto token = components[1];
    auto apiQuery = components[2];

    auto adapter = determineAdapterFromUrl(url);
    if (adapter == nullptr) {
        return strcat(strdup("Failed to determine adapter from url "), url.c_str());
    }

    return strdup(adapter->getIssueList(url, token, apiQuery).c_str());
}

std::shared_ptr<SiteAdapter> QueryEngine::determineAdapterFromUrl(const std::string& site) {
    if (site.find("github.com") != std::string::npos) {
        return std::make_shared<GitHubConnector>();
    }
    return nullptr;
}

} // namespace skye

