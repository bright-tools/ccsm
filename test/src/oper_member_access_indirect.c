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
    simple_struct_t *j = &o;

    j->x = 10;
}

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
