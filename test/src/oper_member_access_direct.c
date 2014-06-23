typedef struct 
{
    int x;
    int y;
} simple_struct_t;

int main( void )
{
    simple_struct_t s = { 1, 2 };

    s.x = 2;
    s.y = 3;

    return 0;
}
