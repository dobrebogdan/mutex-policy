#include <utility>

#include <request.hpp>

using std::move;

namespace mtxpol {

Request::Request(int _id, pid_t _processId, Type _type, MTXPOL_MUTEX _mutexId, Callback _callback):
        id(_id), processId(_processId), type(_type), mutexId(_mutexId), callback(move(_callback)) {}

int Request::getId() const {
    return id;
}

pid_t Request::getProcessId() const {
    return processId;
}

Request::Type Request::getType() const {
    return type;
}

MTXPOL_MUTEX Request::getMutexId() const {
    return mutexId;
}

void Request::resolve(int response) {
    callback(this, response);
}

}  // namespace mtxpol
