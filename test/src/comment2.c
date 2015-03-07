/* comment2.c::Common (with comment.c) header file included */
#include "comment.h"
#include "comment.h"
/* comment2.c::Include it twice */

/* comment2.c::No comments in this include */
#include "comment_free.h"

/* comment2.c::This include file should be excluded */
#include "comment_excluded.h"

/* comment2.c::This is a comment */

// comment2.c::This is a C++ style comment

int x;

typedef struct
{
    unsigned x;
    unsigned y;
} unused_s;

void some_func( void )
{
    unsigned i;

    /* comment2.c::Comment in function some_func */
    if( x < 0 )
    {
    	x++;
    }
    else
    {
        x--;
    }

    for( i = 0; i < 100 ; i++ )
    {
        x++;
	x*=2;
	x++;
	if( x > 10000 )
	{
            x -= 10000;
	}
    }
}

void dummy( void )
{
    /* comment2.c::Comment in function dummy */
    if( x < 0 )
    {
    	x++;
    }
}
