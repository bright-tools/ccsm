#include "simple_include.h"

int x,z,p;

void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int o;
    extern unsigned b;
    static int r;
    register int a;
}

int main( void )
{
    return x;
}
