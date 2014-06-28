void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int t[7];
}

int main( void )
{
    int x[5][2];
    int y[77];

    x[0][0] = 1;
    y[0] = x[0][1];

    return 0;
}
