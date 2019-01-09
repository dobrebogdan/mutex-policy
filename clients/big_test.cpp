#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

#include <mtxpol.hpp>

#include <debug/prettify.hpp>

using mtxpol::prettyResponse;
using std::cin;
using std::cout;


int main()
{
    const int PROCNUM = 100, MUTEX_NUM = 10;
    for(int q = 0; q < PROCNUM; q++)
    {
    int pid = fork();
    if(pid == 0)
    {
      int mutexId = q;
   //   printf("I am process %d, I want mutex %d\n", getpid(), mutexId); 
      fflush(stdout);
      mtxpol_Open(mutexId);
      printf("I am process %d, I opened mutex %d\n",getpid(), mutexId);
      fflush(stdout);
      mtxpol_Lock(mutexId);
      printf("I am process %d, I locked mutex %d\n", getpid(), mutexId);
      fflush(stdout);
      mtxpol_Unlock(mutexId);
      printf("I am process %d, I unlocked mutex %d\n", getpid(), 
      mutexId);
      fflush(stdout);
      mtxpol_Close(mutexId);
      return 0;
      
    }
    }
    //waits until all children are done
    int wpid, status = 0;
    while((wpid = wait(&status)) > 0);

    return 0;
}

