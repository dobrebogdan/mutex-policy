#include <unistd.h>

#include <stdexcept>
#include <tuple>

#include <mutex_policy.hpp>

#define MTXPOL_REENQUEUE_REQUEST -1

using std::lock_guard;
using std::mutex;
using std::pair;
using std::runtime_error;
using std::thread;
using std::tie;

namespace mtxpol {

MutexPolicy::MutexPolicy() {
    requestHandlerThread = new thread(&MutexPolicy::startRequestHandling, this);
    requestResolverThread = new thread(&MutexPolicy::startRequestResolving, this);
}

MutexPolicy::~MutexPolicy() {
    isTerminated = true;
    requestHandlerThread->join();
    requestResolverThread->join();
    delete requestHandlerThread;
    delete requestResolverThread;
}

void MutexPolicy::enqueueRequest(Request* request) {
    requestsQueue.put(request);
}

void MutexPolicy::startRequestHandling() {
    Request* request;
    while (!isTerminated) {
        while (requestsQueue.take(&request)) {
            handleRequest(request);
        }
        usleep(100);
    }
}

void MutexPolicy::startRequestResolving() {
    pair<Request*, int> resolvedRequest;
    while (!isTerminated) {
        while (resolvedRequestsQueue.take(&resolvedRequest)) {
            resolveRequest(resolvedRequest);
        }
        usleep(100);
    }
}

void MutexPolicy::handleRequest(Request* req) {
    int response;
    Request* resolvableLockRequest = nullptr;
    switch (req->getType()) {
        case Request::OPEN:
            response = openMutex(req->getMutexId(), req->getProcessId());
            break;
        case Request::CLOSE:
            response = closeMutex(req->getMutexId(), req->getProcessId());
            break;
        case Request::LOCK:
            response = lockMutex(req->getMutexId(), req->getProcessId());
            break;
        case Request::UNLOCK:
            response = unlockMutex(req->getMutexId(), req->getProcessId());
            if (response == MTXPOL_SUCCESS) {
                // On a successful unlock, check if there are any pending lock
                // requests for this mutex.
                resolvableLockRequest = mutexes[req->getMutexId()]->popPendingLockRequest();
            }
            break;
        default:
            throw runtime_error("Unknown request type.");
    }
    if (response == MTXPOL_REENQUEUE_REQUEST) {
        mutexes[req->getMutexId()]->pushPendingLockRequest(req);
        syscall(331, req->getProcessId());
    } else {
        resolvedRequestsQueue.put({req, response});
    }
    if (resolvableLockRequest != nullptr) {
        // Handle a lock request that was pending in case the mutex it requested
        // has been unlocked.
        handleRequest(resolvableLockRequest);
        syscall(332, resolvableLockRequest -> getProcessId());
    }
}

void MutexPolicy::resolveRequest(pair<Request*, int> resolvedRequest) {
    resolvedRequest.first->resolve(resolvedRequest.second);
    delete resolvedRequest.first;
}

int MutexPolicy::openMutex(MTXPOL_MUTEX mutexId, pid_t processId) {
    if (mutexes.count(mutexId) != 0) {
        return MTXPOL_ERR_MUTEX_ALREADY_OPEN;
    }
    mutexes[mutexId] = new MutexStatus(processId);
    return MTXPOL_SUCCESS;
}

int MutexPolicy::closeMutex(MTXPOL_MUTEX mutexId, pid_t processId) {
    auto mutexPosition = mutexes.find(mutexId);
    if (mutexPosition == mutexes.end()) {
        return MTXPOL_ERR_MUTEX_NOT_OPEN;
    }
    if (mutexPosition->second->isLocked()) {
        return MTXPOL_ERR_MUTEX_LOCKED;
    }
    if (!mutexPosition->second->isCreatedBy(processId)) {
        return MTXPOL_ERR_MUTEX_NOT_OWNED;
    }
    mutexes.erase(mutexPosition);
    return MTXPOL_SUCCESS;
}

int MutexPolicy::lockMutex(MTXPOL_MUTEX mutexId, pid_t processId) {
    auto mutexPosition = mutexes.find(mutexId);
    if (mutexPosition == mutexes.end()) {
        return MTXPOL_ERR_MUTEX_NOT_OPEN;
    }
    if (mutexPosition->second->isLocked()) {
        return MTXPOL_REENQUEUE_REQUEST;
    }
    mutexPosition->second->lock(processId);
    return MTXPOL_SUCCESS;
}

int MutexPolicy::unlockMutex(MTXPOL_MUTEX mutexId, pid_t processId) {
    auto mutexPosition = mutexes.find(mutexId);
    if (mutexPosition == mutexes.end()) {
        return MTXPOL_ERR_MUTEX_NOT_OPEN;
    }
    if (!mutexPosition->second->isLocked()) {
        return MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED;
    }
    if (mutexPosition->second->isLocked(processId)) {
        return MTXPOL_ERR_MUTEX_NOT_OWNED;
    }
    mutexPosition->second->unlock();
    return MTXPOL_SUCCESS;
}

}  // namespace mtxpol
