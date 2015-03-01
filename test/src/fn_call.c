/* File to support
   OP_FN_CALL_CNT
   FUNC_CALLED_BY
   testing
*/

#include "fn_call.h"
#include "empty.h"

extern int foo( void );
extern int moo( void );

void dummy( void )
{
    secondary_function();
}

int j( void )
{
    extern int too( void );

    int x;

    header_function();

    x = foo();
    x+= foo();
    x+= foo();
    x+= moo();
    x+= too();

    secondary_function();
    dummy();

    return x;
}

unsigned int pp;

int k( void )
{
    int x;
    int (*fp)(void) = j;

    x = j();
    x+= j();

    x+= (*fp)();

    return x;
}
