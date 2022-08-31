int path_for_001_expected_2(int y) {
    int x;
    for (x = 0; x < y; x++)
        ;
    /* Summary: path count = 2 (1 + for body) */
    return x;
}

int path_for_003_expected_3(int y) {
    int x, z;
    for (x = 0; x < y; x++)
        for (z = 0; z < x; z++) // Path 1a
        {
            y++; // Path 2a
        }
    // Path 2b - for loop not executed
    // Path 1b - for loop not executed

    /* Summary: path count = [1a2a, 1a2b, 1b] = 3 */
    return x;
}
