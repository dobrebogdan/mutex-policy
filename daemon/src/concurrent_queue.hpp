#ifndef MTXPOL_DAEMON_SRC_CONCURRENT_QUEUE_HPP_
#define MTXPOL_DAEMON_SRC_CONCURRENT_QUEUE_HPP_

#include <mutex>
#include <queue>

namespace mtxpol {

template<class T>
class ConcurrentQueue {
 public:
    void put(const T& obj) {
        std::lock_guard<std::mutex> lock(mut);
        q.push(obj);
    }

    bool take(T* where) {
        std::lock_guard<std::mutex> lock(mut);
        if (q.empty()) {
            return false;
        }
        *where = q.front();
        q.pop();
        return true;
    }

 private:
    std::mutex mut;
    std::queue<T> q;
};

}  // namespace mtxpol

#endif  // MTXPOL_DAEMON_SRC_CONCURRENT_QUEUE_HPP_