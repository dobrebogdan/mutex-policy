#ifndef LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_
#define LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_

/// Type of a mutex descriptor.
///
/// Note: Might change in the future.
#define MTXPOL_MUTEX int

/// Type of request.
///
/// Must be one of `MTXPOL_REQ_OPEN`, `MTXPOL_REQ_CLOSE`, `MTXPOL_REQ_LOCK`, `MTXPOL_REQ_UNLOCK`.
#define MTXPOL_REQ_TYPE int

/// Request type representing a call to `mtxpol_Open`.
#define MTXPOL_REQ_OPEN 0

/// Request type representing a call to `mtxpol_Close`.
#define MTXPOL_REQ_CLOSE 1

/// Request type representing a call to `mtxpol_Lock`.
#define MTXPOL_REQ_LOCK 2

/// Request type representing a call to `mtxpol_Unlock`.
#define MTXPOL_REQ_UNLOCK 3

/// Returned by functions `mtxpol_Open`, `mtxpol_Close`, `mtxpol_Lock` and `mtxpol_Unlock` on success.
#define MTXPOL_SUCCESS 0

/// Returned by `mtxpol_Open` if mutex already exists (was previously opened).
#define MTXPOL_ERR_MUTEX_ALREADY_OPEN 1

/// Returned by `mtxpol_Close`, `mtxpol_Lock` and `mtxpol_Unlock` if mutex does not exist
/// (was already closed or never opened).
#define MTXPOL_ERR_MUTEX_NOT_OPEN 2

/// Returned by `mtxpol_Close` if mutex is locked (`mtxpol_Close` does not allow a locked
/// mutex to be closed).
#define MTXPOL_ERR_MUTEX_LOCKED 3

/// Returned by `mtxpol_Close` if mutex was not created by the same process that is
/// now requesting the close or by `mtxpol_Unlock` if mutex was not locked by the same
/// process that now tries to unlock it.
#define MTXPOL_ERR_MUTEX_NOT_OWNED 4

/// Returned by `mtxpol_Lock` if mutex was not locked when an unlock was requested.
#define MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED 5

/// Returned by `mtxpol_Open`, `mtxpol_Close`, `mtxpol_Lock` and `mtxpol_Unlock` in case
/// they were unable to make the request (likely the mtxpol daemon is not running).
#define MTXPOL_ERR_MTXPOL_NOT_RUNNING 6

/// Used internally in the library to represent that the daemon did not respond yet.
#define MTXPOL_NO_RESPONSE 7

/// Size of a message that is sent to the daemon on every request.
#define MTXPOL_MESSAGE_SIZE (sizeof(int) + sizeof(MTXPOL_REQ_TYPE) + sizeof(pid_t) + sizeof(MTXPOL_MUTEX))

#endif  // LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_
