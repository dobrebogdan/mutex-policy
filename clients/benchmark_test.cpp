#include<unistd.h>
#include<cstdio>
#include<sys/types.h>
#include<sys/wait.h>

#include<mtxpol.hpp>

int main()
{
const int MUTEX_NUM = 1000000;
for(int i = 0; i < MUTEX_NUM; i++)
{
    mtxpol_Open(i+1);
    mtxpol_Lock(i+1);
    printf("%d\n", i+1);
    fflush(stdout); 
    mtxpol_Unlock(i+1);
    mtxpol_Close(i+1);
}

return 0;
}
