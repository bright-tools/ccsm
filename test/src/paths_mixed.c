void path_mixed_001_expected_1(void) {
}

void path_mixed_002_expected_6(int a, int b) {
    switch (a) {
        case 0:
            break;
        case 1:
            break;
    }
    if (b) {
        // b = true
    }
}

void path_mixed_003_expected_5(int a, int b) {
    switch (a) {
        case 0:
            return;
        case 1:
            break;
    }
    if (b) {
        // b = true
    }
}

void path_mixed_004_expected_5(int a, int b) {
    while (a) {
        if (b == 1) {
            return;
        }
        if (b == 2) {
            return;
        }
        if (b == 3) {
            return;
        }
        a--;
    }
}

void path_mixed_005_expected_3(int a, int b, int c) {
    int i;
    for (i = 0; i < 10; i++) {
        if (i > 5) {
            return;
        }
    }
}

void path_mixed_006_expected_8(int a, int b) {
    switch (a) {
        case 0:
            if (a > 0) {
                return;
            }
            break;
        case 1:
            if (a > 10) {
                return;
            }
            break;
        default:
            break;
    }
    if (b) {
        // b = true
    }
}

void path_mixed_007_expected_5(int a, int b) {
    switch (a) {
        case 0:
            return;
        case 1:
            if (a > 10) {
                return;
            }
            break;
        case 2:
            return;
        default:
            break;
    }
}

void path_mixed_008_expected_7(int a, int b) {
    switch (a) {
        case 0:
            return;
        case 1:
            if (a > 10) {
                return;
            }
            break;
        case 2:
            return;
        default:
            break;
    }

    if (b) {
        // b = true
    }
}

void path_mixed_009_expected_4(int a, int b) {
    if (a > 10) {
        while (a) {
            if (b == 1) {
                break;
            }
        }
    }
}

void path_mixed_010_expected_7(int a, int b) {
    if (a > 10) {
        while (a) {
            if (b == 1) {
                continue; // path 1
            }
            if (b == 2) {
                continue; // path 2
            }
            if (b == 3) {
                continue; // path 3
            }

            if (b == 2) {
                break; // can't be reached
            }

            return;  // path 4
        }
        // while not entered: path 5
    }
    // if not entered: path 6
}

void path_mixed_011_expected_7(int a, int b) {
    while (b) {
        switch (a) {
            case 0:
                return;
            case 1:
                switch (a) {
                    case 0:
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        if (b > 0) {
            b--;
        }
    }
}

void path_mixed_012_expected_10(int a, int b) {
    switch (a) {
        case 0:
            if (b)
                break;
            break;
        case 1:
            if (b)
                break;
            break;
        default:
            break;
    }
    if (b > 0) {
        b--;
    }
}

void path_mixed_013_expected_3(int a, int b, int c) {
    switch (a) {
        case 0:
            if (b) {
            }
            break;
    }
}

void path_mixed_014_expected_5(int a, int b, int c) {
    switch (a) {
        case 0:
            if (b) {
            }
            if (c) {
            }
            break;
        case 1:
        default:
            break;
    }
}

void path_mixed_015_expected_5(int a, int b, int c) {
    switch (a) {
        case 0: {
            if (b) {
            }
            if (c) {
            }
            break;
        }
        default:
            break;
    }
}

void path_mixed_016_expected_9(int a, int b, int c) {
    switch (a) {
        case 0:
            return;
        case 1:
            return;
        case 2:
            return;
        case 3:
            return;
        case 4:
            return;
        default:
            break;
    }
    if (b) {

    } else {
    }
    if (c) {

    } else {
    }
}

void path_mixed_017_expected_24(int a, int b, int c) {
    switch (a) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
    if (b) {

    } else {
    }
    if (c) {

    } else {
    }
}

int path_mixed_018_expected_6(int y) {
    int x = 0;
    switch (y) {
        case 10:
        case 9:
        case 0:
            x = 1;
        /* Fall-thru */
        case 1:
            if (y) {
            }
            x++;
            break;
        case 2:
            x = 17;
    }
    /* Summary: path count = 6 (3 different cases, two (due to fall-through)
     * with conditions + implicit default) */

    return x;
}

int path_mixed_019_expected_2(int y) {
    int x;
    for (x = 0; x < y; x++) {
        /* Block 1, path count: 1 */
        continue;

        /* This code is inaccessible, so should not be counted */
        if (x) {
        } else {
        }
    }
    /* Summary: path count = 2 ( Function body + Block 1) */

    return x;
}

int path_mixed_020_expected_2(int y) {
    int x;
    for (x = 0; x < y; x++) {
        /* Block 1, path count: 1 */
        break;

        /* This code is inaccessible, so should not be counted */
        if (x) {
        } else {
        }
    }
    /* Summary: path count = 2 ( Function body + Block 1) */

    return x;
}

#define NEST_MACRO(x, y, z)                                                    \
    do {                                                                       \
        if (x) {                                                               \
            if (y) {                                                           \
                z = 0;                                                         \
            }                                                                  \
        }                                                                      \
    } while (0)

int path_mixed_021_expected_3(int y) {
    unsigned x = 0;

    NEST_MACRO(y, y+1, x);
    return x;
}

void path_mixed_022_expected_2(int y) {
    do {
        if (y) {
            y++;  // Path 1
        }
        // Path 2 - implicit else
    } while (0);
}

void path_mixed_023_expected_3raw_1ca(int y) {
    while (0) {
        if (y) {
            y++;
        }
    }
    // Path 1 - while block is never entered
}

void path_mixed_024_expected_5raw_4ca(int a){
	while (1) {
		if (a == 0){
			break; // 1
		}
		if (a == 1){
			break; // 2
		}
		if (a == 2){
			break; // 3
		}
		break;	// 4
	}
    // while is always entered, no path where it is not
}

void path_mixed_025_expected_5raw_4ca(int a){
	while (1){
		if (a == 0){
			continue; // 1
		}
		if (a == 1){
			continue; // 2
		}
		if (a == 2){
			continue; // 3
		}
		break;	// 4
	}
    // while is always entered, no path where it is not
}

void path_mixed_026_expected_5raw_1ca(int a){
	while (0){
		if (a == 0){
			continue;
		}
		if (a == 1){
			continue;
		}
		if (a == 2){
			continue;
		}
		break;	// 4
	}
    // Path 1 - didn't enter the while
}
