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

int safe( void )
{
    extern int q;
    int lp;
}

/* This variable placed between two functions to check that it doesn't
   get wrapped into one of their stats & is detected as being file-level */
int qq;

int simple( void )
{
    return x + k;
}

#endif
