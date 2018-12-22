#define VERSION "0.0.1"

#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>

#include "mutex_policy.hpp"
#include "request.hpp"

using mtxpol::MutexPolicy;
using mtxpol::Request;
using std::cin;
using std::cout;
using std::string;

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

    auto policy = new MutexPolicy();
    policy->startRequestHandlerThread();

    if (debug) {
        int requestId = 0;
        cout << "Welcome to Mutex Policy's debug shell! The commands available are:\n";
        cout << "\t- open   <mutexId> <processId> - open   mutex <mutexId> as <processId>\n";
        cout << "\t- close  <mutexId> <processId> - close  mutex <mutexId> as <processId>\n";
        cout << "\t- lock   <mutexId> <processId> - lock   mutex <mutexId> as <processId>\n";
        cout << "\t- unlock <mutexId> <processId> - unlock mutex <mutexId> as <processId>\n";
        cout << "\t- terminate - terminate the daemon.\n";
        string input;
        std::vector<int> requests;
        while (cin >> input) {
            if (input == "terminate") {
                cout << "Terminating daemon...\n";
                policy->terminate();
                cout << "Done.\n";
                break;
            } else {
                int id, procId;
                cin >> id >> procId;
                Request::Type type;
                if (input == "open") {
                    type = Request::OPEN;
                } else if (input == "close") {
                    type = Request::CLOSE;
                } else if (input == "lock") {
                    type = Request::LOCK;
                } else if (input == "unlock") {
                    type = Request::UNLOCK;
                } else {
                    cout << "Unknown command '" << input << "'. Not processing.\n";
                    continue;
                }
                policy->enqueueRequest(new Request(
                                ++requestId,
                                procId,
                                type,
                                id,
                                [&requestId](Request* request, int response) {
                    cout << "Request "
                         << request->getId()
                         << " was resolved with response "
                         << response
                         << ".\n";
                }));
            }
        }
    }
    delete policy;
    return 0;
}
