void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int o;
    int *j = &o;

    *j = 3;
}

int main( void )
{
    int x;
    int *y = &x;

    *y = 1;

    return 0;
}
