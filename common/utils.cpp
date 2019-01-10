#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
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

void writeMessage(int fd, void* message, int messageSize) {
    int bytesWritten = 0;
    while (bytesWritten < messageSize) {
        ssize_t chunk = write(fd, (char*)message + bytesWritten, messageSize - bytesWritten);
        if (chunk < 0) {
            perror("write");
            exit(errno);
        }
        bytesWritten += chunk;
    }
}

int readMessage(int fd, void* message, int messageSize, int maxReads) {
    int bytesRead = 0;
    int numReads = 0;
    while (bytesRead < messageSize && (maxReads == -1 || numReads < maxReads)) {
        ssize_t chunk = read(fd, (char*)message + bytesRead, messageSize - bytesRead);
        if (chunk < 0) {
            perror("write");
            exit(errno);
        }
        bytesRead += chunk;
        numReads += 1;
    }
    return bytesRead;
}

int openOrDie(const char* filePath, int mode) {
    int fd = open(filePath, mode);
    if (fd < 0) {
        perror("open");
        exit(errno);
    }
    return fd;
}

}  // namespace mtxpol
