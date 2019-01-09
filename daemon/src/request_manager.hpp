#ifndef DAEMON_SRC_REQUEST_MANAGER_HPP_
#define DAEMON_SRC_REQUEST_MANAGER_HPP_

#include <functional>
#include <string>

#include <request.hpp>

namespace mtxpol {

class RequestManager {
 public:
    typedef std::function<void(Request*)> OnRequestCallback;

    void ensureDirectoryStructure();

    void setOnRequestCallback(const OnRequestCallback& _onRequestCallback);

    void start();

    void terminate();

 private:
    Request* makeRequestFromFile(const std::string& fileName);

    bool ensuredDirectoryStructure = false;
    OnRequestCallback onRequestCallback;

    volatile bool terminated = false;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_REQUEST_MANAGER_HPP_
