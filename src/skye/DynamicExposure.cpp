/**
 * This file contains functions exposed for the dynamic library
 *
 */

#include "skye/QueryEngine.hpp"

#include <cstring>
#include <string>

extern "C" {

/**
 * 
 */
const char* verifyLoaded(int) {
    return "Dynamic library successfully loaded";
}

const char* parseUrl(const char* remote) {
    auto str = skye::QueryEngine::getInstance()->parseUrl(remote);
    return strdup(str.c_str());
}

}
