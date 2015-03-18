#include "recursive_include.h"
#define FUNCTION foo_1
#include "recursive_include2.h"

int main( void )
{
	foo_1();
	foo_2();
}
