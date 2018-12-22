#ifndef DAEMON_SRC_MUTEX_STATUS_HPP_
#define DAEMON_SRC_MUTEX_STATUS_HPP_

#include <sys/types.h>

#include <queue>

#include "request.hpp"

namespace mtxpol {

/// Internal metadata for a Mutex inside the daemon.
class MutexStatus {
 public:
    explicit MutexStatus(pid_t _creator);

    /// Check if mutex is locked by `processId`.
    ///
    /// If `processId == 0`, check if mutex is locked (by anyone).
    bool isLocked(pid_t processId = 0) const;

    /// Check if mutex was created by `processId`.
    bool isCreatedBy(pid_t processId) const;

    /// Mark mutex as locked by process `processId`.
    void lock(pid_t processId);

    /// Mark mutex as unlocked.
    void unlock();

    void pushPendingLockRequest(Request* req);

    Request* popPendingLockRequest();

 private:
    /// PID of process that holds this mutex locked.
    ///
    /// If the mutex is unlocked, this will be 0 instead.
    pid_t locker;

    /// PID of process that opened (created) this mutex.
    pid_t creator;

    /// List of mtxlock requests that could not be handled synchronously because
    /// this mutex was locked.
    ///
    /// When a request comes that unlocks this mutex, one of these requests
    /// should also be resolved.
    std::queue<Request*> pendingLockRequests;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_STATUS_HPP_
