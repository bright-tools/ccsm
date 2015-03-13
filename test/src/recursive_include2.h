
void FUNCTION ( )
{
}

#undef  FUNCTION
#define FUNCTION foo_2
#if !defined FOO2
#define FOO2
#include "recursive_include2.h"
#endif
