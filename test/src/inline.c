inline void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
}

inline void example( void )
{
}

static inline void example2( void )
{
}

int main( void )
{
    return 0;
}
