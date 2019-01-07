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

MutexPolicy::~MutexPolicy() {
    terminate();
    if (requestHandlerThread != nullptr) {
        requestHandlerThread->join();
        delete requestHandlerThread;
    }
    if (requestResolverThread != nullptr) {
        requestResolverThread->join();
        delete requestResolverThread;
    }
}

void MutexPolicy::startRequestHandlerThread() {
    if (requestHandlerThread != nullptr) {
        throw runtime_error("Handler thread started twice!");
    }
    requestHandlerThread = new thread(&MutexPolicy::startRequestHandling, this);
}

void MutexPolicy::startRequestResolverThread() {
    if (requestResolverThread != nullptr) {
        throw runtime_error("Resolver thread started twice!");
    }
    requestResolverThread = new thread(&MutexPolicy::startRequestResolving, this);
}

void MutexPolicy::terminate() {
    isTerminated = true;
}

void MutexPolicy::startRequestHandling() {
    while (!isTerminated) {
        Request* request = extractRequest();
        while (request != nullptr) {
            handleRequest(request);
            request = extractRequest();
        }
        usleep(100);
    }
}

void MutexPolicy::startRequestResolving() {
    while (!isTerminated) {
        Request* request = nullptr;
        int response = 0;
        tie(request, response) = extractResolvedRequest();
        while (request != nullptr) {
            resolveRequest(request, response);
            tie(request, response) = extractResolvedRequest();
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
    } else {
        enqueueResolvedRequest(req, response);
    }
    if (resolvableLockRequest != nullptr) {
        // Handle a lock request that was pending in case the mutex it requested
        // has been unlocked.
        handleRequest(resolvableLockRequest);
    }
}

void MutexPolicy::resolveRequest(Request* req, int resp) {
    req->resolve(resp);
    delete req;
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

Request* MutexPolicy::extractRequest() {
    lock_guard<mutex> guard(requestQueueMutex);
    if (requestQueue.empty()) {
        return nullptr;
    }
    auto request = requestQueue.front();
    requestQueue.pop();
    return request;
}

void MutexPolicy::enqueueRequest(Request* request) {
    lock_guard<mutex> guard(requestQueueMutex);
    requestQueue.push(request);
}

pair<Request*, int> MutexPolicy::extractResolvedRequest() {
    lock_guard<mutex> guard(resolvedRequestsQueueMutex);
    if (resolvedRequestsQueue.empty()) {
        return {nullptr, 0};
    }
    auto ret = resolvedRequestsQueue.front();
    resolvedRequestsQueue.pop();
    return ret;
}

void MutexPolicy::enqueueResolvedRequest(Request* request, int response) {
    lock_guard<mutex> guard(resolvedRequestsQueueMutex);
    resolvedRequestsQueue.push({request, response});
}

}  // namespace mtxpol
