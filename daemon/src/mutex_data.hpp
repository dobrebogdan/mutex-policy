#ifndef DAEMON_SRC_MUTEX_DATA_HPP_
#define DAEMON_SRC_MUTEX_DATA_HPP_

#include <sys/types.h>

#include <cinttypes>

#include <vector>

namespace mtxpol {

struct Mutex {
    enum State: std::uint8_t {
        LOCKED = 0,
        UNLOCKED = 1,
    };

    std::uint32_t id;
    State state;
};

struct MutexRequest {
    enum Operation: std::uint8_t {
        LOCK = 0,
        UNLOCK = 1,
    };

    pid_t processId;
    std::uint32_t mutexId;
    Operation op;
};

struct MutexData {
    std::vector<Mutex> mutexes;
    std::vector<MutexRequest> requests;
};

enum MessagePrefixByte: std::uint8_t {
    DONE = 0,
    MUTEX = 1,
    REQUEST = 2,
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_DATA_HPP_
