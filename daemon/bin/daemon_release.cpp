#include <cstring>

#include <iostream>

#include <daemonize.hpp>
#include <mutex_policy.hpp>
#include <request_manager.hpp>

using mtxpol::daemonize;
using mtxpol::MutexPolicy;
using mtxpol::Request;
using mtxpol::RequestManager;
using std::cout;

int main(int argc, char* argv[]) {

//    daemonize();

    auto policy = new MutexPolicy();
    policy->startRequestHandlerThread();
    policy->startRequestResolverThread();

    auto requestManager = new RequestManager();
    requestManager->ensureDirectoryStructure();
    requestManager->setOnRequestCallback([&policy](Request* request) {
        policy->enqueueRequest(request);
    });
    requestManager->start();

    delete policy;
    return 0;
}
