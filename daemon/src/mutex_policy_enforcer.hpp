#ifndef DAEMON_SRC_MUTEX_POLICY_ENFORCER_HPP_
#define DAEMON_SRC_MUTEX_POLICY_ENFORCER_HPP_

#include "mutex_data.hpp"

namespace mtxpol {

class MutexPolicyEnforcer {
 public:
    void applyPolicy(const MutexData& data);
};

}  // namespace mtxpol

#endif  // DAEMON_SRC_MUTEX_POLICY_ENFORCER_HPP_
