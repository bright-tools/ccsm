#include "kw_return.h"
#include "kw_return.h"

#define FIX_MACRO(_x) do { if( _x > 0 ) { return 0; } } while( 0 )

int err;

void no_return_fn( void )
{
    err = 0;
}

void return_fn( void )
{
    err = 1;
    return;
}

int main( void )
{
    int x = 0;
    int q = 0;
    int t = 5;

    FIX_MACRO( x );
    RETURN( x );

    for( q = 0; q < t; q++ )
    {
        if( q > t )
        {
            return -1;
        }
    }

    return err;
}

