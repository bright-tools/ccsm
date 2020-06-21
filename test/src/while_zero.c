/* Test code for CCSM ( http://dev.brightsilence.com/ccsm/ )
 *
 * (c) 2018 John Bailey
 */

int foo( void )
{
    /* There's only one path through this */
    do
    {}
    while(0);
    /* And only one path through this */
    do
    {}
    while(0 && 0);

    /* Only one path here (jump to end of compound statement) */
    while(0)
    {
        /* This is unreachable */
        unsigned a;
        if( a )
        {
            a++;
        }
        else
        {
            a--;
        }
    }
    return 0;
}