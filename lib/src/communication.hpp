#ifndef MTXPOL_LIB_SRC_COMMUNICATION_HPP_
#define MTXPOL_LIB_SRC_COMMUNICATION_HPP_

namespace mtxpol {

void sendMessageToDaemon(int requestId, void* message);

/// Returns MTXPOL_NO_RESPONSE if the response did not arrive yet,
/// or the response if it did.
int pollForResponse(int requestId);

}

#endif  // MTXPOL_LIB_SRC_COMMUNICATION_HPP_
