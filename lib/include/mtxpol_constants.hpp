#ifndef LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_
#define LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_

/// Type of a mutex descriptor.
///
/// Note: Might change in the future.
#define MTXPOL_MUTEX int

/// Returned by functions Open, Close, Lock and Unlock on success.
#define MTXPOL_SUCCESS 0

/// Returned by Open if mutex already exists (was previously opened).
#define MTXPOL_ERR_MUTEX_ALREADY_OPEN 1

/// Returned by Close, Lock and Unlock if mutex does not exist
/// (was already closed or never opened).
#define MTXPOL_ERR_MUTEX_NOT_OPEN 2

/// Returned by Close if mutex is locked (Close does not allow a locked
/// mutex to be closed).
#define MTXPOL_ERR_MUTEX_LOCKED 3

/// Returned by Close if mutex was not created by the same process that is
/// now requesting the close or by Unlock if mutex was not locked by the same
/// process that now tries to unlock it.
#define MTXPOL_ERR_MUTEX_NOT_OWNED 4

/// Returned by Lock if mutex was not locked when an unlock was requested.
#define MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED 5

#endif  // LIB_INCLUDE_MUTEX_POLICY_CONSTANTS_HPP_
