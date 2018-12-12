#ifndef DAEMON_SRC_DEBUG_MOCK_SYSCALL_LIB_HPP_
#define DAEMON_SRC_DEBUG_MOCK_SYSCALL_LIB_HPP_

#include <sys/types.h>

int mtxopen(int mutexId);

int mtxclose(int mutexId);

int mtxlock(int mutexId);

int mtxunlock(int mutexId);

int mtxlist(void** output);

int mtxgrant(int mutexId, pid_t processId);

#endif  // DAEMON_SRC_DEBUG_MOCK_SYSCALL_LIB_HPP_
