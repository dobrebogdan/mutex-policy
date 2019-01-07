#include <cstring>

#include <iostream>

#include <debug/debug_shell.hpp>
#include <mutex_policy.hpp>

using mtxpol::MutexPolicy;
using mtxpol::openDebugShell;
using std::cout;

int main() {
    auto policy = new MutexPolicy();
    policy->startRequestHandlerThread();
    openDebugShell(policy);
    delete policy;
    return 0;
}
