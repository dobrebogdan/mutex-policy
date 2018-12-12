#include <errno.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>

#include "mtxpol_syscalls.hpp"
#include "data_gatherer.hpp"

using std::cout;
using std::uint8_t;
using std::size_t;

namespace mtxpol {

MutexData DataGatherer::gather() {
    void* rawData = nullptr;
    int retCode = mtxlist(&rawData);
    if (retCode != 0) {
        perror("mtxlist");
        exit(errno);
    }
    auto data = static_cast<uint8_t*>(rawData);
    MutexData parsedData;
    size_t cursor = 0;
    while (data[cursor] != MessagePrefixByte::DONE) {
        switch (data[cursor]) {
            case MessagePrefixByte::MUTEX: {
                Mutex mtx{};
                memcpy(&mtx, data + cursor + 1, sizeof(mtx));
                cursor += 1 + sizeof(mtx);
                parsedData.mutexes.push_back(mtx);
                break;
            }
            case MessagePrefixByte::REQUEST: {
                MutexRequest req{};
                memcpy(&req, data + cursor + 1, sizeof(req));
                cursor += 1 + sizeof(req);
                parsedData.requests.push_back(req);
                break;
            }
            default: {
                cout << "Invalid message received from mtxlist. Prefix byte: "
                     << data[cursor]
                     << "\n";
                exit(1);
            }
        }
    }
    return parsedData;
}

}  // namespace mtxpol
