#pragma once
#include <string>

namespace skye {

class SiteAdapter {
protected:
public:
    virtual ~SiteAdapter() = default;

    virtual std::string getIssueAndComments(int forceRefresh) = 0;
    virtual std::string getIssueList(int forceRefresh) = 0;
};

}
