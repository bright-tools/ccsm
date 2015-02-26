#include "stmt_if.h"

#define CHECK_MACRO(_x) do { if( _x ) { err = 1; } } while( 0 )

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
    }

    return err;
}
