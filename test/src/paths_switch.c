void path_switch_001_expected_3(int a) {
    switch (a) {
        case 0:
            break; // Path 1a
        case 1:
            break; // Path 1b
            // Path 1c - no cases executed
    }

    // Summary: Paths = [1a, 1b, 1c] = 3
}

int path_switch_002_expected_1(int y) {
    int x = 0;
    switch (y)
        ;
    /* Summary: path count = 1 */

    return x;
}

int path_switch_003_expected_1(int y) {
    int x = 0;
    switch (y)
        x++;
    /* Summary: path count = 1 */

    return x;
}

int path_switch_004_expected_2(int y) {
    int x = 0;
    switch (y)
    case 0:
        x++;

    /* Summary: path count = 2 (1 case + implicit default) */

    return x;
}

void path_switch_005_expected_3(int a) {
    switch (a) {
        case 0:
            break; // Path 1a
        case 1:
            break; // Path 1b
        default:
            break; // Path 1c
    }

    // Summary: Paths = [1a, 1b, 1c] = 3
}
