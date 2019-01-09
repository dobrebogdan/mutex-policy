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

    while (cin >> s >> id) {
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
        cout << "Response: " << ret;
        if (ret == -1) {
            cout << " (unknown command).";
        } else {
            cout << " (" << prettyResponse(ret) << ")";
        }
        cout << "\n";
    }

    return 0;
}