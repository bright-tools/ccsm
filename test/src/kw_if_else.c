#include "kw_if_else.h"

#define CHECK_MACRO(_x) do { if( _x ) { err = 1; } else { err = 7; } } while( 0 )

int err;

int main( void )
{
    int x = 0;

    CHECK_MACRO( x );
    IF( x );

    if( x == 0 )
    {
        if( x == 1 )
	{
            err = 0;
	}
	else
	{
            err = 2;
	}
    }

    return err;
}
