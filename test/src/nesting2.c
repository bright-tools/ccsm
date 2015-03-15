#include "nesting.h"

unsigned something( unsigned x, unsigned y, unsigned z )
{
    unsigned ret = 0;
    if( x )
    {
        /* Nesting level 1 */
        switch( y )
        {
            case 1:
                /* Nesting level 2 */
                if( z ) {
                    /* Nesting Level 3 */
                    ret = 4;
                }
                break;
            default:
                /* Nesting level 2 */
                if( z > 5 )
                {
                    /* Nesting level 3 */
                    if ( z < 10 )
                    {
                        /* Nesting level 4 */
                        while( ret < 100 )
                        {
                            unsigned i;
                            /* Nesting level 5 */
                            ret += z;
                            for( i = 0; i < y; i++ )
                            {
                                /* Nesting level 6 */
                                do {
                                    /* Nesting level 7 */
                                    ret += 1;
                                    ret *= 2;
                                } while( ret );
                            }
                        }
                    }
                }
                break;
        }
    }
    return ret;
}
