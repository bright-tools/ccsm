
extern unsigned foo;

#if !defined FOO
#define FOO
#include "recursive_include.h"
#endif
