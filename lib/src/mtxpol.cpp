#include <unistd.h>

#include <mtxpol.hpp>

int mtxpol_Request(MTXPOL_REQ_TYPE requestType, MTXPOL_MUTEX mutexId) {
    pid_t processId = getpid();
}

int mtxpol_Open(MTXPOL_MUTEX mutexId) {
    return mtxpol_Request(MTXPOL_REQ_OPEN, mutexId);
}

int mtxpol_Close(MTXPOL_MUTEX mutexId) {
    return mtxpol_Request(MTXPOL_REQ_CLOSE, mutexId);
}

int mtxpol_Lock(MTXPOL_MUTEX mutexId) {
    return mtxpol_Request(MTXPOL_REQ_LOCK, mutexId);
}

int mtxpol_Unlock(MTXPOL_MUTEX mutexId) {
    return mtxpol_Request(MTXPOL_REQ_UNLOCK, mutexId);
}
