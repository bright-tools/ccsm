#include "linkage.h"

void other_fn()
{
}

static void a_fn()
{
}

void b_fn()
{
}

int main2( void )
{
    a_fn();
    b_fn();
    other_fn();
    return 0;
}
