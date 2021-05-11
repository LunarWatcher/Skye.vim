#pragma once

#include "skye/SiteAdapter.hpp"
#if defined(_MSC_VER)
#define SKYE_EXPORT extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define SKYE_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define SKYE_EXPORT extern "C"
#pragma warning "Couldn't determine export semantics for compiler. Please open an issue, or better, a PR"
#endif

#include <memory>
#include <string>
#include <iostream>

namespace skye {

class QueryEngine {
private:
    static inline QueryEngine* INSTANCE;
    std::shared_ptr<SiteAdapter> adapter;

    std::string url;

public:
    ~QueryEngine() { std::cout << "Deconstructed" << std::endl; }
    void setUrl(const std::string& url);
    std::string parseUrl(const std::string& remoteName);

    const char* queryIssueList(int fr);

    static QueryEngine* getInstance();
    const std::string& getUrl() { return url; }
};

}
