#include <mutex_status.hpp>

namespace mtxpol {

MutexStatus::MutexStatus(pid_t _creator): creator(_creator), locker(0) {}

bool MutexStatus::isLocked(pid_t processId) const {
    return locker != processId;
}

bool MutexStatus::isCreatedBy(pid_t processId) const {
    return creator == processId;
}

void MutexStatus::lock(pid_t processId) {
    locker = processId;
}

void MutexStatus::unlock() {
    locker = 0;
}

void MutexStatus::pushPendingLockRequest(Request* req) {
    pendingLockRequests.push(req);
}

Request* MutexStatus::popPendingLockRequest() {
    if (pendingLockRequests.empty()) {
        return nullptr;
    }
    Request* req = pendingLockRequests.front();
    pendingLockRequests.pop();
    return req;
}

}  // namespace mtxpol
