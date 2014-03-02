extern int z;
int x,t;
int y;

extern int gain();

int main( void )
{
main_start:
    if( x == 1 )
    {
        if( y == 2 )
        {
            if( t == 3 )
            {
                return 2;
            }
        } else {
            goto main_start;
        }
    }
main_end:
    return 1;
}

int k;

static int rain( int i )
{
    return i+1;
}

void vain( int i )
{
    int j;
    extern int p;
    for( j = 0;
         j < 2;
         j++ )
    {
    }
    j = (1 + 3 + 7) / 12;
    j = (j>1)?0:1;
    switch( j )
    {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
    while( j > 0 )
    {
        j--;
    }
    if( 4 == 4 )
    {
        if(( j == 5 ) ||
           ( j == 6 ))
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
    else
    {
        /* This does not count as the final return statement
           as it's inside a conditionally executed compound */
        return;
    }
}
