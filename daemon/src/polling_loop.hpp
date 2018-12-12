#ifndef DAEMON_SRC_POLLING_LOOP_HPP_
#define DAEMON_SRC_POLLING_LOOP_HPP_

#include "data_gatherer.hpp"
#include "mutex_policy_enforcer.hpp"

namespace mtxpol {

class PollingLoop {
 public:
    PollingLoop(DataGatherer* _gatherer, MutexPolicyEnforcer* _enforcer);

    bool isTerminated() const;

    void terminate();

    void step();

    void join();

 private:
    DataGatherer* gatherer;
    MutexPolicyEnforcer* enforcer;

    volatile bool terminated = false;
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_POLLING_LOOP_HPP_
