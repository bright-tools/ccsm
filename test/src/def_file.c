#include "def_file.h"
#include "def_file.h"

typedef enum
{
#define DEF( _x ) ENUM ## _x ,
#include "def_file.def"
#undef DEF
    LAST_DEF
} unused_e;

#define DEF( _x ) void JUNK ## _x ( void ) { return; }
#include "def_file.def"
#undef  DEF

int main( void )
{
    JUNKthere();
    return 0;
}
