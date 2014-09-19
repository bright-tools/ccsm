/* File to support VAR_FN_LOC_AUTO_CNT testing
   2 functions to check that count is correctly associated with each function
*/

int x;
int y;

int main( void )
{
    auto int t;
    int z;
    auto int v;

    if( 1 == 1 )
    {
        auto int j;
    }

    return 0;
}

int t;

void v( void )
{
    auto int p;
    int l;
}
