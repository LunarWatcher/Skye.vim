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

namespace skye {

class QueryEngine {
private:
    static inline std::shared_ptr<QueryEngine> INSTANCE;
    std::shared_ptr<SiteAdapter> adapter;

    std::string url;

public:
    void setUrl(const std::string& url);
    std::string parseUrl(const std::string& remoteName);

    const char* queryIssueList(int fr);

    static std::shared_ptr<QueryEngine> getInstance();
    const std::string& getUrl() { return url; }
};

}
