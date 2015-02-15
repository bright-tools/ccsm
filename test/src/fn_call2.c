#include "fn_call.h"
#include "fn_call2.h"

/* This function shouldn't be confused with the other function
   with the same name but different scope which is in fn_call.c */
static int j()
{
    return 1;
}

void secondary_function( void )
{
    j();
    header_function();
}
