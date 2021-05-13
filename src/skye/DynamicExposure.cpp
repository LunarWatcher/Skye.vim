/**
 * This file contains functions exposed for the dynamic library
 *
 */

// export sematics
#if defined(_MSC_VER)
#define SKYE_EXPORT extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define SKYE_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define SKYE_EXPORT extern "C"
#pragma warning "Couldn't determine export semantics for compiler. Please open an issue, or better, a PR"
#endif

#include "skye/QueryEngine.hpp"
#include "skye/GitHubConnector.hpp"
#include <iostream>

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
    auto str = skye::QueryEngine::parseUrl(remote);
    if (str != "") {
        return strdup(str.c_str());
    } else {
        return "";
    }
}

SKYE_EXPORT const char* getIssues(const char* rawInput) {
    return skye::QueryEngine::queryIssueList(rawInput);
}

}
