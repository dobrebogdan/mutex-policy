#include <cstring>

#include <iostream>

#include <daemonize.hpp>
#include <mutex_policy.hpp>

using mtxpol::daemonize;
using mtxpol::MutexPolicy;
using std::cout;

int main(int argc, char* argv[]) {

    daemonize();

    auto policy = new MutexPolicy();
    policy->startRequestHandlerThread();
    policy->startRequestResolverThread();

    // TODO(darius98): Start looking for input pipes in the ~/.mtxpol folder.

    delete policy;
    return 0;
}
