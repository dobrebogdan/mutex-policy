#include <unistd.h>

#include <stdexcept>
#include <mutex_policy.hpp>

#define MTXPOL_REENQUEUE_REQUEST -1

using std::lock_guard;
using std::mutex;
using std::runtime_error;
using std::thread;

namespace mtxpol {

MutexPolicy::~MutexPolicy() {
    terminate();
    if (requestHandlerThread != nullptr) {
        requestHandlerThread->join();
        delete requestHandlerThread;
    }
}

void MutexPolicy::startRequestHandlerThread() {
    if (requestHandlerThread != nullptr) {
        throw runtime_error("Handler thread started twice!");
    }
    requestHandlerThread = new thread(&MutexPolicy::startRequestHandling, this);
}

void MutexPolicy::terminate() {
    isTerminated = true;
}

void MutexPolicy::enqueueRequest(Request* request) {
    lock_guard<mutex> guard(requestQueueMutex);
    requestQueue.push(request);
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
        req->resolve(response);
        delete req;
    }
    if (resolvableLockRequest != nullptr) {
        // Handle a lock request that was pending in case the mutex it requested
        // has been unlocked.
        handleRequest(resolvableLockRequest);
    }
}

int MutexPolicy::openMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
    if (mutexes.count(mutexId) != 0) {
        return MTXPOL_ERR_MUTEX_ALREADY_OPEN;
    }
    mutexes[mutexId] = new MutexStatus(processId);
    return MTXPOL_SUCCESS;
}

int MutexPolicy::closeMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
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

int MutexPolicy::lockMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
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

int MutexPolicy::unlockMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
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

}  // namespace mtxpol
