void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int o;
    unsigned *j;

    j = (unsigned*)o;
}

int main( void )
{
    int x;
    unsigned *u;
    unsigned *y;
    unsigned *z;

    y = (unsigned*)((unsigned*) z);
    z = (unsigned*) x;
    u = (unsigned*) &x;

    return 0;
}
