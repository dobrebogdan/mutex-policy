#include <stdexcept>

#include <constants.hpp>
#include <debug/prettify.hpp>

using std::runtime_error;
using std::string;

namespace mtxpol {

string prettyRequestType(Request::Type type) {
    switch (type) {
        case Request::OPEN: return "open";
        case Request::CLOSE: return "close";
        case Request::LOCK: return "lock";
        case Request::UNLOCK: return "unlock";
        default: throw runtime_error("Cannot prettify unknown request type.");
    }
}

string prettyResponse(int response) {
    switch (response) {
        case MTXPOL_SUCCESS: return "MTXPOL_SUCCESS";
        case MTXPOL_ERR_MUTEX_NOT_OWNED: return "MTXPOL_ERR_MUTEX_NOT_OWNED";
        case MTXPOL_ERR_MUTEX_NOT_OPEN: return "MTXPOL_ERR_MUTEX_NOT_OPEN";
        case MTXPOL_ERR_MUTEX_ALREADY_OPEN: return "MTXPOL_ERR_MUTEX_ALREADY_OPEN";
        case MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED: return "MTXPOL_ERR_MUTEX_ALREADY_UNLOCKED";
        case MTXPOL_ERR_MUTEX_LOCKED: return "MTXPOL_ERR_MUTEX_LOCKED";
        default: throw runtime_error("Cannot prettify unknown response.");
    }
}

}  // namespace mtxpol
