#include "linkage.h"

static void local_fn()
{
}

static void a_fn()
{
}

static void b_fn()
{
}

int main( void )
{
    /* Local function call */
    local_fn();
    /* Local function call */
    a_fn();
    /* Local function call */
    b_fn();
    /* External function call */
    other_fn();
    return 0;
}
