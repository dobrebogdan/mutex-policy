#ifndef DAEMON_SRC_MTXPOL_SYSCALLS_HPP_
#define DAEMON_SRC_MTXPOL_SYSCALLS_HPP_

#ifdef MOCK_SYSCALLS

#include "debug/mock_syscall_lib.hpp"

#else

#include <unistd.h>

#endif  // MOCK_SYSCALLS


#endif  //DAEMON_SRC_MTXPOL_SYSCALLS_HPP_
