#include <cstdio>

#include <mtxpol.hpp>

int main() {
    printf("%d\n", mtxpol_Open(2));
    printf("%d\n", mtxpol_Lock(2));
    printf("%d\n", mtxpol_Unlock(2));
    printf("%d\n", mtxpol_Close(2));
    return 0;
}
