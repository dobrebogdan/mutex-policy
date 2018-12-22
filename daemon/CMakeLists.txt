cmake_minimum_required(VERSION 3.7)

add_executable(daemon
        src/main.cpp
        src/mutex_policy.cpp
        src/request.cpp
)
target_compile_definitions(daemon PUBLIC -DMTXPOL_TESTING)
target_link_libraries(daemon PUBLIC pthread)
