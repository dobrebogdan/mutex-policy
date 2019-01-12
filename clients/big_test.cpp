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
    for(int i = 0; i < MUTEX_NUM; i++)
    {
       mtxpol_Open(i + 1);
    }
    for(int q = 0; q < PROCNUM; q++)
    {
    int pid = fork();
    if(pid == 0)
    {
      int mutexId = rand() % MUTEX_NUM + 1;
   //   printf("I am process %d, I want mutex %d\n", getpid(), mutexId); 
      fflush(stdout);
      printf("Opening mutex: %d\n",mtxpol_Open(mutexId));
      printf("I am process %d, I opened mutex %d\n",getpid(), mutexId);
      fflush(stdout);
      mtxpol_Lock(mutexId);
      printf("I am process %d, I locked mutex %d\n", getpid(), mutexId);
      fflush(stdout);
      mtxpol_Unlock(mutexId);
      printf("I am process %d, I unlocked mutex %d\n", getpid(), 
      mutexId);
      fflush(stdout);
      printf("Closing mutex: %d\n", mtxpol_Close(mutexId));
      printf("I am process %d, I closed mutex %d\n", getpid(), mutexId);
      fflush(stdout);
      return 0;
      
    }
    }
    //waits until all children are done
    int wpid, status = 0;
    while((wpid = wait(&status)) > 0);
    
    for(int i = 0; i < MUTEX_NUM; i++)
    {
       mtxpol_Close(i+1);
    }
    return 0;
}

