/**
 * This file contains functions exposed for the dynamic library
 *
 */

extern "C" {

/**
 * 
 */
const char* verifyLoaded(int) {
    return "Dynamic library successfully loaded";
}

}
