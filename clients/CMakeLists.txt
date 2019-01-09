add_executable(simple_lock_wait_unlock simple_lock_wait_unlock.cpp)
target_link_libraries(simple_lock_wait_unlock PUBLIC mtxpol)

add_executable(interactive_shell interactive_shell.cpp ../daemon/src/debug/prettify.cpp)
target_link_libraries(interactive_shell PUBLIC mtxpol)
target_include_directories(interactive_shell PUBLIC ../daemon/src)
