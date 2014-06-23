typedef struct 
{
    int x;
    int y;
} simple_struct_t;

int main( void )
{
    simple_struct_t s = { 1, 2 };
    simple_struct_t *p = &s;
    simple_struct_t *q = &s;

    p->x = 2;
    q->x = 3;
    q->y = 4;

    return 0;
}
