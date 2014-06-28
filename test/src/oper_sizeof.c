#include <stdint.h>

void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int o;

    o = sizeof( uint8_t );
}

int main( void )
{
    int x;

    x = sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint32_t );

    return 0;
}
