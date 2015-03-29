#include "kw_return.h"
#include "kw_return.h"

#define FIX_MACRO(_x) do { if( _x > 0 ) { return 0; } } while( 0 )

int err;

void no_return_fn( void )
{
    err = 0;

    /* Implicit return only */
}

void return_fn( void )
{
    err = 1;

    /* Explicit return only */
    return;
}

int all_path_return( int x )
{
    /* All paths have an explicit return */
    if( x )
    {
        return 1;
    }
    else
    {
	return 2;
    }
}

int some_path_return( int x )
{
    if( x > 100 )
    {
	return 1;
    } else if( x < 100 )
    {
        return 0;
    }
    else
    {
        err = 3;
    }
}

int case_path_return( int x )
{
    switch( x )
    {
        case 0:
	    return 10;
	    break;
        case 6:
        case 8:
	    return 9;
	    break;
	default:
	    return 11;
	    break;
    }
}

int case_path_some_return( int x )
{
    switch( x )
    {
        case 0:
	    return 10;
	    break;
        case 6:
        case 8:
	    return 9;
	    break;
    }
}

int inaccessible_code_return( int x )
{
    return 3;
    if( x > 10 )
    {
        return 1;
    }
}

int main( void )
{
    /* All paths have an explicit return */

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

