cmake_minimum_required(VERSION 3.7)

add_executable(daemon
        src/data_gatherer.cpp
        src/main.cpp
        src/mutex_policy_enforcer.cpp
        src/polling_loop.cpp

        src/debug/mock_syscall_lib.cpp
)
target_compile_definitions(daemon PUBLIC -DMOCK_SYSCALLS)
