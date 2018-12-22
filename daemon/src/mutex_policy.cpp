#include <zconf.h>

#include <stdexcept>

#include "mutex_policy.hpp"

#define MTXPOL_REENQUEUE_REQUEST -1

using std::lock_guard;
using std::mutex;
using std::pair;
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
        Request* firstExtractedRequest = request;
        bool firstRequest = true;
        while (request != nullptr &&
               (firstRequest || request != firstExtractedRequest)) {
            handleRequest(request);
            request = extractRequest();
            firstRequest = false;
        }
        usleep(100);
    }
}

void MutexPolicy::handleRequest(Request* req) {
    int response;
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
            break;
        default:
            throw runtime_error("Unknown request type.");
    }
    if (response == MTXPOL_REENQUEUE_REQUEST) {
        enqueueRequest(req);
    } else {
        req->resolve(response);
        delete req;
    }
}

int MutexPolicy::openMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
    if (mutexes.count(mutexId) != 0) {
        return MTXPOL_ERR_MUTEX_ALREADY_OPEN;
    }
    mutexes[mutexId] = MutexStatus{0, processId};
    return MTXPOL_SUCCESS;
}

int MutexPolicy::closeMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
    auto mutexPosition = mutexes.find(mutexId);
    if (mutexPosition == mutexes.end()) {
        return MTXPOL_ERR_MUTEX_NOT_OPEN;
    }
    if (mutexPosition->second.locker != 0) {
        return MTXPOL_ERR_MUTEX_LOCKED;
    }
    if (mutexPosition->second.creator != processId) {
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
    if (mutexPosition->second.locker != 0) {
        return MTXPOL_REENQUEUE_REQUEST;
    }
    mutexPosition->second.locker = processId;
    return MTXPOL_SUCCESS;
}

int MutexPolicy::unlockMutex(MUTEX_DESCRIPTOR mutexId, pid_t processId) {
    auto mutexPosition = mutexes.find(mutexId);
    if (mutexPosition == mutexes.end()) {
        return MTXPOL_ERR_MUTEX_NOT_OPEN;
    }
    if (mutexPosition->second.locker == 0) {
        return MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED;
    }
    if (mutexPosition->second.locker != processId) {
        return MTXPOL_ERR_MUTEX_NOT_OWNED;
    }
    mutexPosition->second.locker = 0;
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
