#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

#include "daemonize.hpp"

namespace mtxpol {

void daemonize() {
    pid_t childPid = fork();

    if (childPid < 0) {
        perror("first fork failed");
        exit(errno);
    }

    if (childPid != 0) {  // main process
        printf("First fork successful. Exiting main process.\n");
        exit(0);
    }

    // child process
    pid_t grandchildPid = fork();

    if (grandchildPid < 0) {
        perror("second fork failed");
        exit(errno);
    }

    if (grandchildPid != 0) {  // first-child process
        printf("Second fork successful. Exiting first-child process.\n");
        exit(0);
    }
}

}  // namespace mtxpol
