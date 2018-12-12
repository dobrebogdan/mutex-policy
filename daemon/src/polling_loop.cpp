#include <zconf.h>

#include "polling_loop.hpp"

namespace mtxpol {

PollingLoop::PollingLoop(DataGatherer* _gatherer,
                         MutexPolicyEnforcer* _enforcer):
     gatherer(_gatherer),
     enforcer(_enforcer) {}

bool PollingLoop::isTerminated() const {
    return terminated;
}

void PollingLoop::terminate() {
    terminated = true;
}

void PollingLoop::step() {
    enforcer->applyPolicy(gatherer->gather());
}

void PollingLoop::join() {
    while (!isTerminated()) {
        step();
        usleep(100);  // microseconds
    }
}

}  // namespace mtxpol
