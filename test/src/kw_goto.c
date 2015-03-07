#include "kw_goto.h"
#include "kw_goto.h"

#define FIX_MACRO(_x) do { if( _x ) { goto err_pt; } } while( 0 )

int goto_fn( void ) 
{
    goto end;
end:
    return 1;
}

int main( void )
{
    unsigned x = 0;

    FIX_MACRO( x );

inc:
    x++;
    if ( x < 100 ) {
        goto inc;
    } else {
	goto end;
    }

end:
    return -1;
err_pt:
    return 0;
}

