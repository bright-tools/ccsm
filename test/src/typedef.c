typedef struct
{
    int x;
} s_t;

void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    typedef int di_t;
}

void t( void )
{
    typedef int i_t;
}

typedef int oi_t;

int main( void )
{
    if( ! 0 ) {}
    return 'a';
}
