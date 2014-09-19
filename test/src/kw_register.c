/* File to support VAR_FN_LOC_REG_CNT & TOK_REGISTER testing
   2 functions to check that count is correctly associated with each function
*/

/* Can't have file-scope "register" variables */
int x;
extern int y;
int q;
int x;

int main( void )
{
    auto int t;
    /* Same name as file-scope var */
    register int x;
    extern int o;

    if( 1 == 1 )
    {
        register int j;
    }

    return 0;
}

int t;

void v( void )
{
    register int p;
    int l;
    int b;
}
