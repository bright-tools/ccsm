void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */

    int o;
    int *j;

    j = &o;
}

int main( void )
{
    int x;
    int *y;
    int t;
    int *q;

    y = &x;
    q = &t;

    return 0;
}
