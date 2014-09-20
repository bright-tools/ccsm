/* File to support
   OP_FN_CALL_CNT
   testing
*/

extern int foo( void );
extern int moo( void );

int j( void )
{
    extern int too( void );

    int x;

    x = foo();
    x+= foo();
    x+= foo();
    x+= moo();
    x+= too();

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
