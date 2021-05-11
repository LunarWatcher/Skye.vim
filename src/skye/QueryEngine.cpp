#include "QueryEngine.hpp"
#include "GitHubConnector.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <array>
#include <sstream>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace skye {

void QueryEngine::setUrl(const std::string &url) {
    if (url.find("github.com") != std::string::npos) {
        // The provider is GitHub
        this->adapter = std::make_shared<GitHubConnector>();
    }
    this->url = url;
}

std::string QueryEngine::parseUrl(const std::string &remoteName) {
    std::array<char, 128> buff;
    std::string res;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("git remote -v", "r"), pclose);
    if (!pipe) {
        return "";
    }
    while (fgets(buff.data(), buff.size(), pipe.get()) != nullptr) {
        res += buff.data();
    }
    // And convert that trash ^ to C++
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

QueryEngine* QueryEngine::getInstance() {
    if (INSTANCE == nullptr) {
        INSTANCE = new QueryEngine();
        std::cout << "new instance" << std::endl;
    }
    return INSTANCE;
}

const char* QueryEngine::queryIssueList(int fr) {
    if (this->adapter == nullptr) {
        setUrl(parseUrl("origin"));
        if (adapter == nullptr) {
            // TODO: revisit the setting API
            return "Adapter not set, and failed to find one for remote 'origin'. Please set it explicitly";
        }
    }
    return strdup(adapter->getIssueList(fr).c_str());
}

}

