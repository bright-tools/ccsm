#include "kw_for.h"

#define FIX_MACRO(_x) do { unsigned i; for( i = 0; i < _x ; i++ ) { err += _x; } } while( 0 )

int err;

int main( void )
{
    int x = 0;
    int q,r;
    int t = 0;

    FIX_MACRO( x );
    FOR( x );

    for( q = 0; q < x; q++ )
    {
        for( r = 0 ; r < q ; r++ )
	{
            t++;
	}
    }

    return err;
}

