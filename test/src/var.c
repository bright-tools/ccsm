int x,z,p;
int q;
extern int t,l;

void dummy( void )
{
    /* This function shouldn't be counted due to the
       --exclude-fuction parameter setting */
    int o;
    extern unsigned b;
    static int r;
    register int a;
}

void example( void )
{
    int x;
    int z;
    extern int q;
    static int b;
    register int o;
}

/* This var is placed after a function to ensure that
   it does not get included in the function's count */
static int j;

int main( void )
{
    int x;
    int y;
    int p;
    extern int z;
    extern int v;
    auto int h;

    return 0;
}
