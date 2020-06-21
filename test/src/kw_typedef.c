#include "kw_typedef.h"

#define TYPEDEF_FACTORY( _x ) typedef short _x

typedef long lng;

void sample( void )
{
    typedef short sht;
    TYPEDEF_FACTORY( td_z );
}

typedef int fint;

void example( void )
{
    typedef short sht;
    typedef double dbl;
}

TYPEDEF_FACTORY( td_a );
TYPEDEF_FACTORY( td_b );
TYPEDEF_FACTORY( td_c );
TYPEDEF_FACTORY( td_d );
