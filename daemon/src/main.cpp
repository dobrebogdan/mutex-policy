#define VERSION "0.0.1"

#include <unistd.h>

#include <cstdlib>
#include <cstdio>

#include <iostream>

using std::cout;

void daemonize() {
    pid_t childPid = fork();

    if (childPid < 0) {
        perror("first fork failed");
        exit(errno);
    }

    if (childPid != 0) {  // main process
        cout << "First fork successful. Exiting main process.\n";
        exit(0);
    }

    // child process
    pid_t grandchildPid = fork();

    if (grandchildPid < 0) {
        perror("second fork failed");
        exit(errno);
    }

    if (grandchildPid != 0) {  // first-child process
        cout << "Second fork successful. Exiting first-child process.\n";
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    cout << "Inter-process mutex policy daemon, version " << VERSION << "\n";
    cout << "Initializing daemon process...\n";

    daemonize();



    return 0;
}
