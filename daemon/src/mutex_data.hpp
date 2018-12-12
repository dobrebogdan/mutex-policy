#ifndef DAEMON_SRC_MUTEX_DATA_HPP_
#define DAEMON_SRC_MUTEX_DATA_HPP_

#include <sys/types.h>

#include <cinttypes>

#include <vector>

namespace mtxpol {

struct Mutex {
    enum State: std::int8_t {
        LOCKED = 0,
        UNLOCKED = 1,
    };

    std::int32_t id;
    State state;
};

struct MutexRequest {
    enum Operation: std::int8_t {
        LOCK = 0,
        UNLOCK = 1,
    };

    pid_t processId;
    std::int32_t mutexId;
    Operation op;
};

struct MutexData {
    std::vector<Mutex> mutexes;
    std::vector<MutexRequest> requests;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_DATA_HPP_