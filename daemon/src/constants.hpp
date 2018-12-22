#ifndef DAEMON_SRC_CONSTANTS_HPP_
#define DAEMON_SRC_CONSTANTS_HPP_

#define MUTEX_DESCRIPTOR int

/// Returned by functions mtxopen, mtxclose, mtxlock and mtxunlock on success.
#define MTXPOL_SUCCESS 0

/// Returned by mtxopen if mutex already exists (was previously opened).
#define MTXPOL_ERR_MUTEX_ALREADY_OPEN 1

/// Returned by mtxclose, mtxlock and mtxunlock if mutex does not exist
/// (was already closed or never opened).
#define MTXPOL_ERR_MUTEX_NOT_OPEN 2

/// Returned by mtxclose if mutex is locked (mtxclose does not allow a locked
/// mutex to be closed).
#define MTXPOL_ERR_MUTEX_LOCKED 3

/// Returned by mtxclose if mutex was not created by the same process that is
/// now requesting the close or by mtxunlock if mutex was not locked by the same
/// process that now tries to unlock it.
#define MTXPOL_ERR_MUTEX_NOT_OWNED 4

/// Returned by mtxlock if mutex was not locked when an unlock was requested
#define MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED 5

#endif  // DAEMON_SRC_CONSTANTS_HPP_
