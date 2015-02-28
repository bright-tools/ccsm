#if !defined KW_FOR_H
#define      KW_FOR_H

/* This is some for() in a comment */

#define L_LIMIT 2
#define ERR_INC 5

#define FOR( _for ) do { unsigned char l; for( l = 0; l < L_LIMIT; l++ ) { err = (err + ERR_INC ) * 2; } } while( 0 )

extern int err;

inline int x_for( int y_for )
{
    int ret_val = 0;
    int i;

    FOR( y_for );

    for( i = 0; i < y_for; i++ )
    {
        int j;
	for( j = 0; j < y_for; j++ )
	{
            ret_val *= 2;
	}
	ret_val += 1;
    }

    return ret_val;
}

#endif

