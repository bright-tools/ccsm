unsigned j = 10 / 2;

int main(void) {
}

void arithmetic(void) {
    int x = 0;

    x = x + 1;
    x = x - 1;
    x = x / 1;
    x = x * 1;
    x = +x;
    x = -x;
    x = x % 2;
    x++;
    x--;
    ++x;
    --x;
}

void arithmetic_init_add(void) {
    int x = 1 + 1;
}

void arithmetic_init_sub(void) {
    int x = 1 - 1;
}

void arithmetic_init_mul(void) {
    int x = 1 * 1;
}

void arithmetic_init_div(void) {
    int x = 1 / 1;
}

void arithmetic_init_mod(void) {
    int x = 1 % 1;
}

void arithmetic_init_sizeof(void) {
    int x = sizeof(int);
}

void arithmetic_init_log_not(void) {
    int x = !1;
}

void comp(void) {
    int x = 0;
    int y;

    y = (x == 0);
    y = (x != 0);
    y = (x < 0);
    y = (x > 0);
    y = (x >= 0);
    y = (x <= 0);
}

void logic(void) {
    int x = 0;

    x = 1 || 1;
    x = 1 && 1;
    x = !1;
}

void bitwise(void) {
    unsigned x = 0;

    x = ~1u;
    x = 1u & 1u;
    x = 1u | 2u;
    x = 2u ^ 1u;
    x = 2u >> 1;
    x = 2u << 1;
}

void assign(void) {
    unsigned x = 0;

    x += 2;
    x -= 2;
    x *= 2;
    x /= 2;
    x %= 2;
    x &= 2;
    x |= 2;
    x ^= 2;
    x <<= 2;
    x >>= 2;
}

void fn_call(void) {
    /* Function call operator */
    assign();
}

void array(void) {
    unsigned x[2];
    unsigned *y;

    x[0] = 1;
    y = &(x[0]);
    x[0] = *y;
}

void structs(void) {
    typedef struct {
        unsigned x;
    } tester_t;

    tester_t x;
    /* TODO: check up on this - in this context, the use of '=' as an part of
     *       the initialiser - should this be counted as an operator? */
    tester_t *z = &x;

    x.x = 5;
    z->x = 4;
}

void misc(void) {
    unsigned x = 0;

    x = x ? (unsigned)sizeof(x) : 1;
    x++, x++;
}
