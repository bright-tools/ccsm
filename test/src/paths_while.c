int path_while_001_expected_2(int y) {
    int x;
    while (++x < y)
        ;
    /* Summary: path count = 2 (1 + while body) */
    return x;
}

void path_while_002_expected_1(int y) {
    do {
        y++;
    } while (1 - 1);
}

void path_while_003_expected_2_raw_1ca()
{
    while (0) {
    }
}

void path_while_004_expected_3(int a, int b)
{
    while (a) {
        // Path 1a
        while (b) {
            // Path 2a
        }
        // Path 2b - while not entered 
    }
    // Path 1b - while not entered

    /* Summary: paths = [1a2a, 1a2b, 1b] = 3 */
}

void path_while_005_expected_3raw_1ca(int a, int b)
{
    while (0) {
        // Path 1a
        while (b) {
            // Path 2a
        }
        // Path 2b - while not entered 
    }
    // Path 1b - while not entered

    /* Summary: raw paths = [1a2a, 1a2b, 1b] = 3 */
    /*          condition analysis paths = [1b] = 1 */
}

void path_while_006_expected_3raw_2ca(int a, int b)
{
    while (1) {
        // Path 1a
        while (b) {
            // Path 2a
        }
        // Path 2b - while not entered 
    }
    // Path 1b - while not entered

    /* Summary: raw paths = [1a2a, 1a2b, 1b] = 3 */
    /*          condition analysis paths = [1a2a, 1a2b] = 2 */
}

void path_while_007_expected_2(int y) {
    do {
        // Path 1a
        while (y) {
            // Path 2a
        }
        // Path 2b - while not entered
    } while (0);

    /* Summary: raw paths = [1a2a, 1a2b] = 2 */
}

int path_while_008_expected_1(void)
{
    do {
        // Path 1a
    } while(0);

    do {
        // Path 1a
    } while(0);
}

int path_while_009_expected_3raw_1ca(void) {
    /* There's only one path through this */
    do {
        // Path 1a
    } while (0);
    /* And only one path through this */
    do {
        // Path 1a
    } while (0 && 0);

    while (0) {
        /* This is technically unreachable */
        unsigned a;
        while (a)
        {
            // Path 2a
        }
        // Path 2b - while not entered
    }
    // Path 2c - while not entered

    // Summary: Raw paths = [1a2a, 1a2b, 1a2c] = 3
    //          With condition analysis = [1a2c] = 1
    return 0;
}