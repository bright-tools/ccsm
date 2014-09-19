/* File to support
   VAR_FILE_EXT_CNT, VAR_FN_EXT_CNT,
   FUNC_EXTERN_EXPL_CNT & FUNC_EXTERN_IMPL_CNT & METRIC_TYPE_TOKEN_EXTERN
   testing
   2 functions to check that count is correctly associated with each function
*/

extern int x;
extern int y;
int q;
int j;

extern int foo( int bob );
extern int oof( int bob );
extern int ofo( int bob );
int ambig( int bob );

int main( void )
{
    auto int t;
    /* Same variable name as file-scope extern - also, as this text is in a
       comment it shouldn't be counted as a token */
    extern int x;
    auto int v;
    auto int a;

    if( 1 == 1 )
    {
        extern int j;
    }

    return 0;
}

extern int t;

void v( void )
{
    extern int p;
    extern int l;
    int b;
}
