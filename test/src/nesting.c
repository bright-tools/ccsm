#include "nesting.h"

#if 0
void dummy(void) {
    unsigned j;

    NEST_MACRO(1, 2, j);
}

#endif

unsigned elifer(unsigned x, unsigned y, unsigned z) {
    unsigned ret = 0;

    if (x < 10) {
        /* Nesting level 1 */
        ret = 0;
    } else if (x > 100) {
        /* Nesting level 1 */
        if (y) {
            /* Nesting level 2 */
            ret = 100;
        } else if (z) {
            /* Nesting level 2 */
            ret = 200;
        }
    } else {
        /* Nesting level 1 */
        ret = 50;
    }

    return ret;
}

unsigned singleIf(unsigned y) {
    unsigned ret = 0;
    if (y) {
        ret = 1;
    }
    return ret;
}

unsigned elser(unsigned x, unsigned y, unsigned z) {
    unsigned ret = 0;
    if (x) {
        /* Nesting level 1 */
        ret = 1;
    } else {
        /* Nesting level 1 */
        ret = 2;
        if (y) {
            /* Nesting level 2 */
            ret = 2;
        } else {
            ret = 6;
            /* Nesting level 2 */
            if (z) {
                /* Nesting level 3 */
                ret = 3;
            } else {
                /* Nesting level 3 */
                do {
                    /* Nesting level 4 */
                    ret = 4;
                } while (0);
            }
        }
    }
    /* Make sure that a 1-level nest at the end of the function doesn't
       effect a previously accumulated maximum nest level */
    if (y > ret) {
        ret = 0;
    }
    return ret;
}

#if 0
int main(void) {
    unsigned x = 0;

    NEST_MACRO(1, 5, x);
    return x;
}
#endif