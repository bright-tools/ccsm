unsigned int x;

/* TODO: Don't think that various of the counts work at FN level!
 * Don't deal with dupes within fns (e.g. 2 fns with different variable
 * instantiations)
 */

void empty(void) {
    /* Deliberately empty */
}

inline int storage(void) /* Don't count function declaration part     */
{                        /*                                           */
    typedef int p;       /* HALSTEAD_OPERATOR_UNIQUE_CNT:  1 - typedef         */
                         /* HALSTEAD_OPERATOR_CNT:         1 - typedef         */
                         /* HALSTEAD_OPERAND_UNIQUE_CNT:   2 - int, p          */
                         /* HALSTEAD_OPERAND_CNT:          2 - int, p          */
    auto int x;          /* HALSTEAD_OPERATOR_UNIQUE_CNT:  2 - auto            */
                         /* HALSTEAD_OPERATOR_CNT:         2 - auto            */
                         /* HALSTEAD_OPERAND_UNIQUE_CNT:   3 - x               */
                         /* HALSTEAD_OPERAND_CNT:          4 - int, x          */
    extern int y;        /* HALSTEAD_OPERATOR_UNIQUE_CNT:  3 - extern          */
                         /* HALSTEAD_OPERATOR_CNT:         3 - extern          */
                         /* HALSTEAD_OPERAND_UNIQUE_CNT:   4 - y               */
                         /* HALSTEAD_OPERAND_CNT:          6 - int, y          */
    register int t;      /* HALSTEAD_OPERATOR_UNIQUE_CNT:  4 - register        */
                         /* HALSTEAD_OPERATOR_CNT:         4 - register        */
                         /* HALSTEAD_OPERAND_UNIQUE_CNT:   5 - t               */
                         /* HALSTEAD_OPERAND_CNT:          8 - int, t          */
    static int l;        /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5 - static          */
                         /* HALSTEAD_OPERATOR_CNT:         5 - static          */
                         /* HALSTEAD_OPERAND_UNIQUE_CNT:   6 - l               */
                         /* HALSTEAD_OPERAND_CNT:         10 - int, l          */
}

int qual(void)      /* Don't count function declaration part              */
{                   /*                                                    */
    const int p;    /* HALSTEAD_OPERATOR_UNIQUE_CNT:  1 - const           */
                    /* HALSTEAD_OPERATOR_CNT:         1 - const           */
                    /* HALSTEAD_OPERAND_UNIQUE_CNT:   2 - int, p          */
                    /* HALSTEAD_OPERAND_CNT:          2 - int, p          */
    volatile int x; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  2 - volatile        */
                    /* HALSTEAD_OPERATOR_CNT:         2 - volatile        */
                    /* HALSTEAD_OPERAND_UNIQUE_CNT:   3 - x               */
                    /* HALSTEAD_OPERAND_CNT:          4 - int, x          */
}

void arith(int a, int b) {
    int c; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  0                   */
           /* HALSTEAD_OPERATOR_CNT:         0                   */
           /* HALSTEAD_OPERAND_UNIQUE_CNT:   2 - int, c          */
           /* HALSTEAD_OPERAND_CNT:          2 - int, c          */

    c = a + b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  2 - =, +            */
               /* HALSTEAD_OPERATOR_CNT:         2 - =, +            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4 - a, b            */
               /* HALSTEAD_OPERAND_CNT:          5 - a, b, c         */

    c = a - b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  3 - -               */
               /* HALSTEAD_OPERATOR_CNT:         4 - =, -            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:          8 - a, b, c         */

    c = a / b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  4 - /               */
               /* HALSTEAD_OPERATOR_CNT:         6 - =, /            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         11 - a, b, c         */

    c = a * b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5 - *               */
               /* HALSTEAD_OPERATOR_CNT:         8 - =, *            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         14 - a, b, c         */

    /* Repeats of the above */
    c = a + b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
               /* HALSTEAD_OPERATOR_CNT:        10 - =, +            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         17 - a, b, c         */

    c = a - b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
               /* HALSTEAD_OPERATOR_CNT:        12 - =, -            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         20 - a, b, c         */

    c = a / b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
               /* HALSTEAD_OPERATOR_CNT:        14 - =, /            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         23 - a, b, c         */

    c = a * b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
               /* HALSTEAD_OPERATOR_CNT:        16 - =, *            */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
               /* HALSTEAD_OPERAND_CNT:         26 - a, b, c         */
}

void arith_assign(int a, int b) {
    int c = a; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  1 - =               */
               /* HALSTEAD_OPERATOR_CNT:         1 - =               */
               /* HALSTEAD_OPERAND_UNIQUE_CNT:   3 - int, c, a       */
               /* HALSTEAD_OPERAND_CNT:          3 - int, c, a       */

    c += b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  2 - +=              */
            /* HALSTEAD_OPERATOR_CNT:         2 - +=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4 - b               */
            /* HALSTEAD_OPERAND_CNT:          5 - b, c            */

    c -= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  3 - -=              */
            /* HALSTEAD_OPERATOR_CNT:         3 - -=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:          7 - b, c            */

    c /= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  4 - /=              */
            /* HALSTEAD_OPERATOR_CNT:         4 - /=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:          9 - b, c            */

    c *= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5 - *=              */
            /* HALSTEAD_OPERATOR_CNT:         5 - *=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:         11 - b, c            */

    /* Repeats of the above */
    c += b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
            /* HALSTEAD_OPERATOR_CNT:         6 - +=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:         13 - b, c            */

    c -= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
            /* HALSTEAD_OPERATOR_CNT:         7 - -=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:         15 - b, c            */

    c /= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
            /* HALSTEAD_OPERATOR_CNT:         8 - /=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:         17 - b, c            */

    c *= b; /* HALSTEAD_OPERATOR_UNIQUE_CNT:  5                   */
            /* HALSTEAD_OPERATOR_CNT:         9 - *=              */
            /* HALSTEAD_OPERAND_UNIQUE_CNT:   4                   */
            /* HALSTEAD_OPERAND_CNT:         19 - b, c            */
}
