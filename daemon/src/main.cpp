#include <cstring>

#include <iostream>

#include "daemonize.hpp"
#include "mutex_policy.hpp"
#include "debug/debug_shell.hpp"

using mtxpol::daemonize;
using mtxpol::MutexPolicy;
using mtxpol::openDebugShell;
using std::cout;

int main(int argc, char* argv[]) {
    bool debug = (argc >= 2 && strcmp(argv[1], "debug") == 0);

    if (!debug) {
        daemonize();
    }

    auto policy = new MutexPolicy();
    policy->startRequestHandlerThread();

    if (debug) {
        openDebugShell(policy);
    }

    delete policy;
    return 0;
}
