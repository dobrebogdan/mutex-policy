#include <unistd.h>
#include <pwd.h>

#include "utils.hpp"

using std::string;

namespace mtxpol {

string home() {
    const char* homeDirCStr = getenv("HOME");
    if (homeDirCStr == nullptr) {
        homeDirCStr = getpwuid(getuid())->pw_dir;
    }
    string homeDir(homeDirCStr);
    if (homeDir[homeDir.length() - 1] != '/') {
        homeDir += '/';
    }
    return homeDir;
}

string mtxpolHome() {
    return home() + ".mtxpol/";
}

string mtxpolInDir() {
    return mtxpolHome() + "in/";
}

string mtxpolOutDir() {
    return mtxpolHome() + "out/";
}

}  // namespace mtxpol
