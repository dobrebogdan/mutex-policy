#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>

#include <mtxpol_constants.hpp>
#include <utils.hpp>

#include "communication.hpp"

using std::string;
using std::to_string;

namespace mtxpol {

void sendMessageToDaemon(int requestId, void* message) {
    string fileName = mtxpolInDir() + to_string(requestId);

    int status = mkfifo(fileName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    if (status != 0) {
        perror("mkfifo");
        exit(errno);
    }

    int fd = open(fileName.c_str(), O_WRONLY);
    int bytesWritten = 0;
    while (bytesWritten < MTXPOL_MESSAGE_SIZE) {
        ssize_t chunk = write(fd, (char*)message + bytesWritten, MTXPOL_MESSAGE_SIZE - bytesWritten);
        if (chunk < 0) {
            perror("write");
            exit(errno);
        }
        bytesWritten += chunk;
    }
    close(fd);
}

int pollForResponse(int requestId) {
    string fileName = mtxpolOutDir() + to_string(requestId);

    struct stat statInfo{};
    int status = stat(fileName.c_str(), &statInfo);
    if (status != 0 || !S_ISFIFO(statInfo.st_mode)) {
        return MTXPOL_NO_RESPONSE;
    }

    int fd = open(fileName.c_str(), O_RDONLY);
    int response = MTXPOL_NO_RESPONSE;
    int bytesRead = 0;
    while (bytesRead < sizeof(int)) {
        ssize_t chunk = read(fd, (char*)(&response) + bytesRead, sizeof(int) - bytesRead);
        if (chunk < 0) {
            perror("read");
            exit(errno);
        }
        bytesRead += chunk;
    }
    close(fd);
    remove(fileName.c_str());
    return response;
}

}
