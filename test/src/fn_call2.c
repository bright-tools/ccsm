#include "fn_call.h"
#include "fn_call2.h"

typedef void ( *fn_p_t ) ( int val );

fn_p_t fn_ptrs[ 2 ];

/* This function shouldn't be confused with the other function
   with the same name but different scope which is in fn_call.c */
static int j()
{
    return 1;
}

void secondary_function( void )
{
    fn_ptrs[0]( 1 );
    j();
    header_function();
}
