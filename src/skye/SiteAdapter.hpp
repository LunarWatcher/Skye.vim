#pragma once
#include <string>

namespace skye {

class SiteAdapter {
protected:
public:
    virtual ~SiteAdapter() = default;

    // Issues
    virtual std::string getIssueAndComments(const std::string& url, const std::string& token, const std::string& issueId) = 0;
    virtual std::string getIssueList(const std::string& url, const std::string& token, const std::string& apiParameters) = 0;

    // Pull requests
    // Optional to implement for adapters, since issues don't always imply pull requests in the same location
    // Looking at you, Jira
    virtual std::string getPullList(const std::string&, const std::string&, const std::string&) { return "Not supported by this provider"; };
    virtual std::string getPullComments(const std::string&, const std::string&, const std::string&) { return "Not supported by this provider"; };
};

}
