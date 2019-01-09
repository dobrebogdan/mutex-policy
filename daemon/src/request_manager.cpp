#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <vector>

#include <utils.hpp>
#include <request_manager.hpp>

using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

namespace {

void ensureDir(const string &path) {
    int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    if (ret < 0 && errno != EEXIST) {
        perror("mkdir");
        exit(errno);
    }
}

vector<string> findInputFiles() {
    struct dirent* dirp;
    string folderName = mtxpol::mtxpolInDir();
    DIR* dp = opendir(folderName.c_str());
    if (dp == nullptr) {
        perror("opendir");
        exit(errno);
    }

    vector<string> files;
    dirp = readdir(dp);
    while (dirp != nullptr) {
        struct stat statInfo{};
        string fileName = folderName + dirp->d_name;
        int ret = stat(fileName.c_str(), &statInfo);
        if (ret != 0) {
            perror("stat");
            exit(errno);
        }
        // Regular files and named pipes are supported.
        if ((S_ISREG(statInfo.st_mode) || S_ISFIFO(statInfo.st_mode))) {
            files.emplace_back(fileName.c_str());
        }
        dirp = readdir(dp);
    }
    closedir(dp);
    return files;
}

void respond(mtxpol::Request* request, int response) {
    int requestId = request->getId();
    string outputFifoName = mtxpol::mtxpolOutDir() + to_string(requestId);
    int status = mkfifo(outputFifoName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    if (status != 0) {
        perror("mkfifo");
        exit(errno);
    }
    int fd = mtxpol::openOrDie(outputFifoName.c_str(), O_WRONLY);
    mtxpol::writeMessage(fd, &response, sizeof(response));
    close(fd);
}

}

namespace mtxpol {

void RequestManager::ensureDirectoryStructure() {
    // ensure the existence of the ~/.mtxpol, ~/.mtxpol/in/, ~/.mtxpol/out/ folders.
    string homeDir = home();
    ensureDir(mtxpolHome());
    ensureDir(mtxpolInDir());
    ensureDir(mtxpolOutDir());
    ensuredDirectoryStructure = true;
}

void RequestManager::setOnRequestCallback(const OnRequestCallback& _onRequestCallback) {
    if (onRequestCallback) {
        throw runtime_error("RequestManager::Trying to set onRequestCallback twice!");
    }
    onRequestCallback = _onRequestCallback;
}

void RequestManager::start() {
    if (!onRequestCallback) {
        throw runtime_error("RequestManager::Trying to start without setting onRequestCallback!");
    }
    if (!ensuredDirectoryStructure) {
        throw runtime_error("RequestManager::Trying to start without ensuring directory structure!");
    }

    while (!terminated) {
        vector<string> files = findInputFiles();
        for (const string& file: files) {
            Request* req = makeRequestFromFile(file);
            if (req != nullptr) {
                remove(file.c_str());
                onRequestCallback(req);
            }
        }
        usleep(100); // 0.1ms
    }
}

void RequestManager::terminate() {
    terminated = true;
}

Request* RequestManager::makeRequestFromFile(const string& fileName) {
    int fd = openOrDie(fileName.c_str(), O_RDONLY);
    int requestId;
    pid_t processId;
    MTXPOL_REQ_TYPE type;
    MTXPOL_MUTEX mutexId;
    void* in = alloca(MTXPOL_MESSAGE_SIZE);
    int bytesRead = readMessage(fd, in, MTXPOL_MESSAGE_SIZE, 100);
    close(fd);
    if (bytesRead < MTXPOL_MESSAGE_SIZE) {
        return nullptr;
    }
    requestId = *((int*)in);
    type = *((MTXPOL_REQ_TYPE*)((char*)in + sizeof(requestId)));
    processId = *((pid_t*)((char*)in + sizeof(requestId) + sizeof(type)));
    mutexId = *((MTXPOL_MUTEX*)((char*)in + sizeof(requestId) + sizeof(type) + sizeof(processId)));
    Request::Type t;
    switch (type) {
        case MTXPOL_REQ_OPEN: t = Request::OPEN; break;
        case MTXPOL_REQ_CLOSE: t = Request::CLOSE; break;
        case MTXPOL_REQ_LOCK: t = Request::LOCK; break;
        case MTXPOL_REQ_UNLOCK: t = Request::UNLOCK; break;
        default: return nullptr;
    }
    return new Request(requestId, processId, t, mutexId, respond);
}

}  // namespace mtxpol
