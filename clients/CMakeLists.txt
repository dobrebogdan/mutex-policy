add_executable(simple_lock_wait_unlock simple_lock_wait_unlock.cpp)
target_link_libraries(simple_lock_wait_unlock PUBLIC mtxpol)

add_executable(interactive_shell interactive_shell.cpp ../daemon/src/debug/prettify.cpp)
target_link_libraries(interactive_shell PUBLIC mtxpol)
target_include_directories(interactive_shell PUBLIC ../daemon/src)

add_executable(big_test big_test.cpp ../daemon/src/debug/prettify.cpp)
target_link_libraries(big_test PUBLIC mtxpol)
target_include_directories(big_test PUBLIC ../daemon/src)
