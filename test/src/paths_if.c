void path_if_001_expected_2(int a) {
    if (a) {
        // Path 1a
    }
    // Path 1b - implicit else

    // Summary: Paths = [1a, 1b] = 2
}

void path_if_002_expected_4(int a, int b) {
    if (a) {
        // Path 1a
    }
    // Path 1b - implicit else
    if (b) {
        // Path 2a
    }
    // Path 2b - implicit else

    // Summary: Paths = [1a2a, 1a2b, 1b2a, 1b2b] = 4
}

void path_if_003_expected_3(int a, int b) {
    if (a) {
        return; // Path 1a
    }
    // Path 1b - implicit else
    if (b) {
        // Path 2a
    }
    // Path 2b - implicit else

    // Summary: Paths = [1a, 1b2a, 1b2b] = 3
}

void path_if_004_expected_4(int a, int b) {
    if (a) {
        // Path 1a
    }
    // Path 1b - implicit else
    if (b) {
        return; // Path 2a
    }
    // Path 2b - implicit else

    // Summary: Paths = [1a2a, 1a2b, 1b2a, 1b2b] = 4
}

void path_if_005_expected_5(int a, int b, int c) {
    if (a) {
        return; // Path 1a
    }
    // Path 1b - implicit else

    if (b) {
        // Path 2a
    }
    // Path 2b - implicit else
    if (c) {
        // Path 3a
    }
    // Path 3b - implicit else

    // Summary: Paths = [1a, 1b2a3a, 1b2a3b, 1b2b3a, 1b2b3b] = 5
}

// ab, !ab,  a!bc, a!b!c,  !a!bc, !a!b!c
void path_if_006_expected_6(int a, int b, int c) {
    if (a) {
        // a = true
    }
    //
    if (b) {
        // b = true
        return;
    }
    //
    if (c) {
        // c = true
    }
}

void path_if_007_expected_8(int a, int b, int c) {
    if (a) {
        // a = true
    }
    //
    if (b) {
        // b = true
    }
    //
    if (c) {
        // c = true
        return;
    }
}

void path_if_008_expected_4(int a, int b, int c) {
    if (a) {
        // a = true
    }
    //
    else if (b) {
        // b = true
    }
    //
    else if (c) {
        // c = true
    }
}
void path_if_009_expected_4(int a, int b, int c) {
    if (a) {
        return;
    } else {
        if (b) {
            return;
        } else {
            if (c) {
                // c = true
            } else {
                // c = false
            }
        }
    }
}

int path_if_010_expected_2(int y) {
    int x = y;
    if (x) {
        /* Block 1, path count: 1 */
    } else {
        /* Block 2, path count: 1 */
    }
    /* Summary: path count = 2 (Block 1 + Block 2) */

    return x;

    /* This code is inaccessible, so should not be counted */
    if (x) {
    } else {
    }
}

void path_if_011_expected_3raw_1ca() {
    if (1) {
        // Path 1
    } else if (1) {
        // Inaccessible
    } else {
        // Inaccessible
    }
}

void path_if_012_expected_3raw_1ca() {
    if (0) {
        // Inaccessible
    } else if (0) {
        // Inaccessible
    } else {
        // Path 1
    }
}

void path_if_013_expected_2raw_1ca() {
    if (1) {
        // Path 1
    } else {
        // Inaccessible
    }
}

void path_if_014_expected_2raw_1ca() {
    if (1) {
        // Path 1
    }
    // No implicit else
}
