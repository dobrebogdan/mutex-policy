#include <iostream>
#include <string>

#include <mtxpol.hpp>

#include <debug/prettify.hpp>

using mtxpol::prettyResponse;
using std::cin;
using std::cout;
using std::string;

int main() {
    string s;
    int id;

    cout << "This is a simple interactive shell for testing mutexes from the MutexPolicy library.\n";
    cout << "It assumes that the daemon is running and accepting requests in Release mode.\n";
    cout << "Commands accepted are:\n";
    cout << "\t- open <mutex-id>\n";
    cout << "\t- close <mutex-id>\n";
    cout << "\t- lock <mutex-id>\n";
    cout << "\t- unlock <mutex-id>\n";
    cout << "\t- exit\n";
    cout << "$> ";
    while (cin >> s) {
        if (s == "exit") {
            break;
        }
        cin >> id;
        int ret = -1;
        if (s == "open") {
            ret = mtxpol_Open(id);
        } else if (s == "close") {
            ret = mtxpol_Close(id);
        } else if (s == "lock") {
            ret = mtxpol_Lock(id);
        } else if (s == "unlock") {
            ret = mtxpol_Unlock(id);
        }
        if (ret == -1) {
            cout << "Unknown command.";
        } else {
            cout << ret << " (" << prettyResponse(ret) << ")";
        }
        cout << "\n";
        cout << "$> ";
    }

    return 0;
}