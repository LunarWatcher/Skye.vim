/**
 * This file contains functions exposed for the dynamic library
 *
 */

#include "skye/QueryEngine.hpp"
#include "skye/GitHubConnector.hpp"

#include <cstring>
#include <string>

extern "C" {

/**
 * 
 */
SKYE_EXPORT const char* verifyLoaded(int) {
    return "Dynamic library successfully loaded";
}

SKYE_EXPORT const char* detectUrlFromRemote(const char* remote) {
    auto str = skye::QueryEngine::getInstance()->parseUrl(remote);
    if (str != "") {
        skye::QueryEngine::getInstance()->setUrl(str);
        return strdup(str.c_str());
    } else {
        return std::strcat(strdup("Failed to determine URL from remote: "), remote);
    }
}

SKYE_EXPORT const char* setUrl(const char* url) {
    skye::QueryEngine::getInstance()->setUrl(url);
    return nullptr;
}

SKYE_EXPORT const char* getIssues(int forceRefresh) {
    return skye::QueryEngine::getInstance()->queryIssueList(forceRefresh);
}

SKYE_EXPORT const char* loadGHToken(const char* token) {
    skye::GitHubConnector::setToken(token);
    return nullptr;
}

}
