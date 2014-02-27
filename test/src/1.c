int main( void )
{
    if( 1 == 1 )
    {
        if( 2 == 2 )
        {
            if( 3 == 3 )
            {
            }
        }
    }
    return 1;
}

void vain( int i )
{
    int j;
    for( j = 0;
         j < 2;
         j++ )
    {
    }
    if( 4 == 4 )
    {
        if( 5 == 5 )
        {
        } else if( 7 == 9 )
        {
        } else
        {
#if defined THIS_IS_DEFINED
            if( 9 == 1 )
            {
            }
#endif
#if defined THIS_IS_NOT_DEFINED
            for( j = 0;
                 j < 2;
                 j++ )
            {
            }
#endif
        }
    }
}
