cmake_minimum_required(VERSION 3.7)

add_library(daemon_lib
        ../common/utils.cpp

        src/daemonize.cpp
        src/mutex_policy.cpp
        src/mutex_status.cpp
        src/request.cpp
        src/request_manager.cpp

        src/debug/debug_shell.cpp
        src/debug/prettify.cpp
)
target_include_directories(daemon_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(daemon_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include)
target_include_directories(daemon_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)

add_executable(daemon_release bin/daemon_release.cpp)
target_link_libraries(daemon_release PUBLIC pthread daemon_lib)

add_executable(daemon_debug bin/daemon_debug.cpp)
target_compile_definitions(daemon_debug PUBLIC -DMTXPOL_TESTING)
target_link_libraries(daemon_debug PUBLIC pthread daemon_lib)
