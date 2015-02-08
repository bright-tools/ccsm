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

void dummy( void )
{
    /* comment2.c::Comment in function dummy */
}
