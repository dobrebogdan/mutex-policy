#ifndef DAEMON_SRC_DEBUG_PRETTIFY_HPP_
#define DAEMON_SRC_DEBUG_PRETTIFY_HPP_

#include <string>

#include <request.hpp>

namespace mtxpol {

std::string prettyRequestType(Request::Type type);

std::string prettyResponse(int response);

}

#endif  // DAEMON_SRC_DEBUG_PRETTIFY_HPP_
