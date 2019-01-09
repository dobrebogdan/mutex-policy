#ifndef DAEMON_SRC_MUTEX_POLICY_HPP_
#define DAEMON_SRC_MUTEX_POLICY_HPP_

#include <sys/types.h>

#include <map>
#include <thread>

#include <concurrent_queue.hpp>
#include <mtxpol_constants.hpp>
#include <mutex_status.hpp>
#include <request.hpp>

namespace mtxpol {

/// Main daemon class.
///
/// This creates a 3-thread process, in a consumer-producer style:
/// - The first thread (the one that instantiates the daemon) adds requests to the daemon
///   through calls to `enqueueRequest`. These requests are placed in a queue (`requestsQueue`).
/// - The second thread consumes requests from the `requestsQueue`, handles them (decides the output),
///   and enqueues them further in the `resolvedRequestsQueue`.
/// - The third thread consumes the resolved requests from the `resolvedRequestsQueue` and actually
///   sends the response back to the requesting client (by calling `Request::resolve(response)`).
///
/// This system is in place to allow the second thread (the actual mutex policy) to run as fast as possible,
/// since the main thread and the resolver thread can be scaled a lot easier (by running multiple threads
/// instead of one, for example), while the mutex policy has to stay mainly single-threaded.
class MutexPolicy {
 public:
    MutexPolicy();

    ~MutexPolicy();

    void enqueueRequest(Request* request);

 private:
    /// Start handling requests on the current thread.
    void startRequestHandling();

    /// Start resolving requests on the current thread.
    void startRequestResolving();

    void handleRequest(Request* req);

    void resolveRequest(std::pair<Request*, int> resolvedRequest);

    /// Handle process `processId` closing mutex `mutexId`.
    int openMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` opening mutex `mutexId`.
    int closeMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` locking mutex `mutexId`.
    int lockMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    /// Handle process `processId` unlocking mutex `mutexId`.
    int unlockMutex(MTXPOL_MUTEX mutexId, pid_t processId);

    std::map<MTXPOL_MUTEX, MutexStatus*> mutexes;

    ConcurrentQueue<Request*> requestsQueue;
    ConcurrentQueue<std::pair<Request*, int>> resolvedRequestsQueue;

    std::thread* requestHandlerThread = nullptr;
    std::thread* requestResolverThread = nullptr;

    volatile bool isTerminated = false;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_POLICY_HPP_
