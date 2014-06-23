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
