#include "func_path.h"

int t1( int y )
{
    int x = y;
    if( x )
    {
        /* Block 1, path count: 1 */
    } else if( x )
    {
        /* Block 2, path count: 1 */
    }
    /* Summary: path count = 3 (Block 1 + Block 2 + implicit 'else') */

    return x;
}

int t2( int y )
{
    int x;
    for( x = 0; x < y; x++ );
    /* Summary: path count = 2 (1 + for body) */
    return x;
}

int t3( int y )
{
    int x;
    while( ++x < y );
    /* Summary: path count = 2 (1 + while body) */
    return x;
}

int t4( int y )
{
    int x = y;
    if( x )
    {
        /* Block 1, path count: 1 */
    } else
    {
        /* Block 2, path count: 1 */
    }
    /* Summary: path count = 2 (Block 1 + Block 2) */

    return x;
}

int t5( int y )
{
    int x = 0;
    switch( y )
    {
	case 10:
        case 0:
		x = 1;
	case 1:
		x++;
		break;
	case 2:
		x = 17;
    }
    /* Summary: path count = 4 (3 different cases + implicit default) */

    return x;
}

int t6( int y )
{
    int x = 0;
    switch( y )
    {
	case 10:
	case 99:
	default:
        case 0:
		x = 1;
	case 1:
		x++;
		break;
    }
    /* Summary: path count = 2 (2 different cases) */

    return x;
}
