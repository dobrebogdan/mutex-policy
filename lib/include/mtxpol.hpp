#ifndef LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_
#define LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_

#include "mtxpol_constants.hpp"

int mtxpol_Request(MTXPOL_REQ_TYPE requestType, MTXPOL_MUTEX mutexId);

int mtxpol_Open(MTXPOL_MUTEX mutexId);

int mtxpol_Close(MTXPOL_MUTEX mutexId);

int mtxpol_Lock(MTXPOL_MUTEX mutexId);

int mtxpol_Unlock(MTXPOL_MUTEX mutexId);

#endif  // LIB_INCLUDE_MUTEX_POLICY_MUTEX_POLICY_HPP_
