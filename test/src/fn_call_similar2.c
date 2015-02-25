/* This pair of files (fn_call_similar1.c and fn_call_similar2.c) are
 * intended to be the same in terms of the byte positioning of the contents;
 * only the function names and some operations are different.  This
 * is intended to check that the function map is generated correctly
 * and the position is resolved both on file AND position, rather than simply
 * position - i.e. that 2 functions having the same position in different 
 * files are not confused */

int j;

static void p( void )
{
    j--;
}

static void o( void )
{
    j--;
}

int y( void )
{
    p();
    o();
    return j;
}

