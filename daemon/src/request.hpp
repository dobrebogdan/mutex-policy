#ifndef DAEMON_SRC_REQUEST_HPP_
#define DAEMON_SRC_REQUEST_HPP_

#include <sys/types.h>

#include <functional>

#include <constants.hpp>

namespace mtxpol {

/// The internal representation of a request inside the daemon.
class Request {
 public:
    typedef std::function<void(Request*, int)> Callback;

    /// Possible types of request.
    enum Type {
        LOCK,
        UNLOCK,
        OPEN,
        CLOSE
    };

    Request(int _id,
            pid_t _processId,
            Type _type,
            MUTEX_DESCRIPTOR _mutexId,
            Callback _callback);

    int getId() const;

    pid_t getProcessId() const;

    Type getType() const;

    MUTEX_DESCRIPTOR getMutexId() const;

    void resolve(int response);

 private:
    /// ID of request (required to know where to send the response).
    int id;

    /// PID of the requesting process.
    pid_t processId;

    /// Type of the request.
    Type type;

    /// Descriptor of affected mutex.
    MUTEX_DESCRIPTOR mutexId;

    /// Callback to be executed when the request is resolved.
    ///
    /// The integer values passed to it will be (in order):
    ///   - the request's ID
    ///   - the response to the request
    Callback callback;
};


}  // namespace mtxpol

#endif  // DAEMON_SRC_REQUEST_HPP_
