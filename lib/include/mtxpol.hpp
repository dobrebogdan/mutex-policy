#ifndef LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_
#define LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_

#include "mtxpol_constants.hpp"

int mtxpol_Open(int processId, MTXPOL_MUTEX mutexId);

int mtxpol_Close(int processId, MTXPOL_MUTEX mutexId);

int mtxpol_Lock(int processId, MTXPOL_MUTEX mutexId);

int mtxpol_Unlock(int processId, MTXPOL_MUTEX mutexId);

#endif  // LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_
