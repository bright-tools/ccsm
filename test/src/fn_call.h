#if !defined FN_CALL_H
#define      FN_CALL_H

extern void secondary_function( void );

void header_function( void )
{
    secondary_function();
}

#endif
