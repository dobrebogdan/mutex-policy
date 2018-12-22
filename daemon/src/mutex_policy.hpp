#ifndef DAEMON_SRC_MUTEX_POLICY_HPP_
#define DAEMON_SRC_MUTEX_POLICY_HPP_

#include <sys/types.h>

#include <map>
#include <mutex>
#include <queue>
#include <thread>

#include "constants.hpp"
#include "request.hpp"
#include "mutex_status.hpp"

namespace mtxpol {

/// Main daemon class.
class MutexPolicy {
 public:
    ~MutexPolicy();

    /// Start handling requests on a new thread.
    void startRequestHandlerThread();

    /// Terminate the thread loops.
    void terminate();

    void enqueueRequest(Request* request);

 private:
    /// Start handling requests on the current thread.
    void startRequestHandling();

    void handleRequest(Request* req);

    /// Handle process `processId` closing mutex `mutexId`.
    int openMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId);

    /// Handle process `processId` opening mutex `mutexId`.
    int closeMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId);

    /// Handle process `processId` locking mutex `mutexId`.
    int lockMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId);

    /// Handle process `processId` unlocking mutex `mutexId`.
    int unlockMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId);

    Request* extractRequest();

    std::map<MUTEX_DESCRIPTOR, MutexStatus*> mutexes;

    std::mutex requestQueueMutex;
    std::queue<Request*> requestQueue;

    std::thread* requestHandlerThread = nullptr;
    volatile bool isTerminated = false;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_POLICY_HPP_
