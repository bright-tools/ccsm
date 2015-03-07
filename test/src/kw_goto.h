#if !defined KW_GOTO_H
#define      KW_GOTO_H

/* This is some goto in a comment */

#define GOTO( _goto ) do { if( _goto ) { goto end; } } while( 0 )

extern int err;

inline int x_goto( int y_goto )
{
    int ret_val = 0;
    int i;

    GOTO( y_goto );

    goto end;

end:
    return ret_val;
}

#endif

