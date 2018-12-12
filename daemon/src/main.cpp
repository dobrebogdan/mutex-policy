#define VERSION "0.0.1"

#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>

#include "data_gatherer.hpp"
#include "mutex_policy_enforcer.hpp"
#include "polling_loop.hpp"

using std::cout;
using mtxpol::DataGatherer;
using mtxpol::MutexPolicyEnforcer;
using mtxpol::PollingLoop;

void daemonize() {
    pid_t childPid = fork();

    if (childPid < 0) {
        perror("first fork failed");
        exit(errno);
    }

    if (childPid != 0) {  // main process
        cout << "First fork successful. Exiting main process.\n";
        exit(0);
    }

    // child process
    pid_t grandchildPid = fork();

    if (grandchildPid < 0) {
        perror("second fork failed");
        exit(errno);
    }

    if (grandchildPid != 0) {  // first-child process
        cout << "Second fork successful. Exiting first-child process.\n";
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    cout << "Inter-process mutex policy daemon, version " << VERSION << "\n";
    cout << "Initializing daemon process...\n";

    bool debug = (argc >= 2 && strcmp(argv[1], "debug") == 0);
    if (!debug) {
        daemonize();
    } else {
        cout << "Running in debug mode, not daemonizing the process.\n";
    }

    auto dataGatherer = new DataGatherer();
    auto policyEnforcer = new MutexPolicyEnforcer();
    auto loop = new PollingLoop(dataGatherer, policyEnforcer);

    loop->join();

    delete loop;
    delete policyEnforcer;
    delete dataGatherer;

    return 0;
}
