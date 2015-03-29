#if !defined KW_RETURN_H
#define      KW_RETURN_H

/* This is some return() in a comment */

#define RETURN( _return ) do { if ( _return < 0 ) { return 0; } } while( 0 )

extern int err;

inline int x_return( int y_return )
{
    /* Return point 1 */
    RETURN( y_return );

    if( y_return > 100 )
    {
	/* Return point 2 */
        return 1;
    } else if( y_return > 50 )
    {
	/* Return point 3 */
	return 2;
    } else
    {
	/* Return point 4 */
	return 3;
    }

    /* No implicit return - all paths have a return point */
}

#endif

