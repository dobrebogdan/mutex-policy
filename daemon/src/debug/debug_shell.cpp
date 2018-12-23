#include <iostream>
#include <string>

#include "debug_shell.hpp"
#include "prettify.hpp"

using std::cin;
using std::cout;
using std::string;

namespace {

void message(mtxpol::Request* request, int response) {
    // TODO(darius98): Make this better.
    cout << "\r";
    cout << "Request "
         << request->getId()
         << " (Process "
         << request->getProcessId()
         << " "
         << mtxpol::prettyRequestType(request->getType())
         << "s mutex "
         << request->getMutexId()
         << ")"
         << " resolved with response "
         << response
         << " ("
         << mtxpol::prettyResponse(response)
         << ").\n";
    cout << "mtxpol$> ";
    cout.flush();
}

}  // namespace

namespace mtxpol {

void openDebugShell(MutexPolicy* policy) {
    int requestId = 0;
    cout << "Welcome to Mutex Policy's debug shell! The commands available are:\n";
    cout << "\t- open   <mutexId> <processId> - open   mutex <mutexId> as <processId>\n";
    cout << "\t- close  <mutexId> <processId> - close  mutex <mutexId> as <processId>\n";
    cout << "\t- lock   <mutexId> <processId> - lock   mutex <mutexId> as <processId>\n";
    cout << "\t- unlock <mutexId> <processId> - unlock mutex <mutexId> as <processId>\n";
    cout << "\t- exit - terminate the daemon.\n";
    string cmd;
    cout << "mtxpol$> ";
    while (cin >> cmd) {
        if (cmd == "exit") {
            cout << "Exiting...\n";
            policy->terminate();
            break;
        } else {
            int id, procId;
            cin >> id >> procId;
            Request::Type type;
            if      (cmd == "open")   type = Request::OPEN;
            else if (cmd == "close")  type = Request::CLOSE;
            else if (cmd == "lock")   type = Request::LOCK;
            else if (cmd == "unlock") type = Request::UNLOCK;
            else {
                cout << "Unknown command '" << cmd << "'. Not processing.\n";
                continue;
            }
            policy->enqueueRequest(new Request(++requestId, procId, type, id, message));
        }
        cout << "mtxpol$> ";
    }
}

}  // namespace mtxpol
