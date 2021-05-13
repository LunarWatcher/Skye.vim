#pragma once
#include <string>

namespace skye {

class SiteAdapter {
protected:
public:
    virtual ~SiteAdapter() = default;

    virtual std::string getIssueAndComments(const std::string& url, const std::string& token, const std::string& issueId) = 0;
    virtual std::string getIssueList(const std::string& url, const std::string& token, const std::string& apiParameters) = 0;
};

}
