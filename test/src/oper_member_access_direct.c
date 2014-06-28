typedef struct 
{
    int x;
    int y;
} simple_struct_t;

void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */

    simple_struct_t o = { 3, 4 };
    o.x = 10;
}

int main( void )
{
    simple_struct_t s = { 1, 2 };

    s.x = 2;
    s.y = 3;

    return 0;
}
