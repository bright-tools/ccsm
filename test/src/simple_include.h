#if !defined SIMPLE_INCLUDE_H
#define      SIMPLE_INCLUDE_H

extern int x;
int k;

void dummy2( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int j;
}

int simple( void )
{
    return x + k;
}

#endif
