#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

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

    int fd = openOrDie(fileName.c_str(), O_WRONLY);

    writeMessage(fd, message, MTXPOL_MESSAGE_SIZE);

    close(fd);
}

int pollForResponse(int requestId) {
    string fileName = mtxpolOutDir() + to_string(requestId);

    struct stat statInfo{};
    int status = stat(fileName.c_str(), &statInfo);
    if (status != 0 || !S_ISFIFO(statInfo.st_mode)) {
        return MTXPOL_NO_RESPONSE;
    }

    int fd = openOrDie(fileName.c_str(), O_RDONLY);

    int response;
    readMessage(fd, &response, sizeof(response));
    close(fd);
    remove(fileName.c_str());
    return response;
}

}
