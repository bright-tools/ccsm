#if !defined DEF_FILE_H
#define      DEF_FILE_H

#define DEF( _x ) unsigned _x;
#include "def_file.def"
#undef  DEF

#define DEF( _x ) void EMPTY ## _x ( void ) { }
#include "def_file.def"
#undef  DEF

#define DEF( _x ) void FUNC ## _x ( void ) { return; }
#include "def_file.def"
#undef  DEF

#endif

