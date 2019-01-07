#ifndef DAEMON_SRC_MUTEX_POLICY_HPP_
#define DAEMON_SRC_MUTEX_POLICY_HPP_

#include <sys/types.h>

#include <map>
#include <mutex>
#include <queue>
#include <thread>

#include <request.hpp>
#include <mtxpol_constants.hpp>
#include <mutex_status.hpp>

namespace mtxpol {

/// Main daemon class.
class MutexPolicy {
 public:
    ~MutexPolicy();

    /// Start handling requests on a new thread.
    void startRequestHandlerThread();

    /// Start the thread that calls callbacks and cleans up resolved requests.
    void startRequestResolverThread();

    /// Terminate the thread loops.
    void terminate();

    void enqueueRequest(Request* request);

 private:
    /// Start handling requests on the current thread.
    void startRequestHandling();

    /// Start resolving requests on the current thread.
    void startRequestResolving();

    void handleRequest(Request* req);

    void resolveRequest(Request* req, int resp);

    /// Handle process `processId` closing mutex `mutexId`.
    int openMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` opening mutex `mutexId`.
    int closeMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` locking mutex `mutexId`.
    int lockMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` unlocking mutex `mutexId`.
    int unlockMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    Request* extractRequest();

    std::pair<Request*, int> extractResolvedRequest();

    void enqueueResolvedRequest(Request* request, int response);

    std::map<MTXPOL_MUTEX, MutexStatus*> mutexes;

    std::mutex requestQueueMutex;
    std::queue<Request*> requestQueue;

    std::mutex resolvedRequestsQueueMutex;
    std::queue<std::pair<Request*, int>> resolvedRequestsQueue;

    std::thread* requestHandlerThread = nullptr;
    std::thread* requestResolverThread = nullptr;

    volatile bool isTerminated = false;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_POLICY_HPP_
