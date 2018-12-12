#ifndef DAEMON_SRC_DATA_GATHERER_HPP_
#define DAEMON_SRC_DATA_GATHERER_HPP_

#include "mutex_data.hpp"

namespace mtxpol {

class DataGatherer {
 public:
    MutexData gather();
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_DATA_GATHERER_HPP_
