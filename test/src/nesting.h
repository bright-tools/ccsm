#if !defined NESTING_H
#define      NESTING_H

#define NEST_MACRO( x, y, z) do { if( x ) { if( y ) { z = 0; } } } while( 0 )

#endif
