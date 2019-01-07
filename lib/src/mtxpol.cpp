#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include <mtxpol.hpp>
#include "communication.hpp"
#include "random.hpp"

using mtxpol::getRandomInteger;
using mtxpol::sendMessageToDaemon;
using mtxpol::pollForResponse;

void* buildRequest(int requestId, MTXPOL_REQ_TYPE requestType, pid_t processId, MTXPOL_MUTEX mutexId) {
    int cursor = 0;
    void* message = malloc(MTXPOL_MESSAGE_SIZE);

    memcpy(static_cast<char*>(message) + cursor, &requestId, sizeof(int));
    cursor += sizeof(int);

    memcpy(static_cast<char*>(message) + cursor, &requestType, sizeof(MTXPOL_REQ_TYPE));
    cursor += sizeof(MTXPOL_REQ_TYPE);

    memcpy(static_cast<char*>(message) + cursor, &processId, sizeof(pid_t));
    cursor += sizeof(pid_t);

    memcpy(static_cast<char*>(message) + cursor, &mutexId, sizeof(MTXPOL_MUTEX));

    return message;
}

int mtxpol_Request(MTXPOL_REQ_TYPE requestType, MTXPOL_MUTEX mutexId) {
    int requestId = getRandomInteger();
    pid_t processId = getpid();

    void* message = buildRequest(requestId, requestType, processId, mutexId);

    sendMessageToDaemon(requestId, message);
    int response = pollForResponse(requestId);
    while (response == MTXPOL_NO_RESPONSE) {
        response = pollForResponse(requestId);
        usleep(100);  // microseconds (0.1ms)
    }
    return response;
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
