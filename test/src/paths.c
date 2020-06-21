
void path_001_expected_1(void){
}

void path_002_expected_2(int a){
    if (a){
        // a = true
    }
}

void path_003_expected_4(int a, int b){
    if (a){
        // a = true
    }
    //
    if (b){
        // b = true
    }
}

void path_004_expected_3(int a, int b){
    if (a){
        // a = true
        return;
    }
    //
    if (b){
        // b = true
    }
}

void path_005_expected_4(int a, int b){
    if (a){
        // a = true
    }
    //
    if (b){
        // b = true
        return;
    }
}

void path_006_expected_5(int a, int b, int c){
    if (a){
        // a = true
        return;
    }
    //
    if (b){
        // b = true
    }
    //
    if (c){
        // c = true
    }
}

// ab, !ab,  a!bc, a!b!c,  !a!bc, !a!b!c
void path_007_expected_6(int a, int b, int c){
    if (a){
        // a = true
    }
    //
    if (b){
        // b = true
        return;
    }
    //
    if (c){
        // c = true
    }
}

void path_008_expected_8(int a, int b, int c){
    if (a){
        // a = true
    }
    //
    if (b){
        // b = true
    }
    //
    if (c){
        // c = true
        return;
    }
}

void path_009_expected_4(int a, int b, int c){
    if (a){
        // a = true
    }
    //
    else if (b){
        // b = true
    }
    //
    else if (c){
        // c = true
    }
}

void path_010_expected_3(int a){
    switch (a){
        case 0:
            break;
        case 1:
            break;
    }
}

void path_011_expected_6(int a, int b){
    switch (a){
        case 0:
            break;
        case 1:
            break;
    }
    if (b){
        // b = true
    }
}

void path_012_expected_5(int a, int b){
    switch (a){
        case 0:
            return;
        case 1:
            break;
    }
    if (b){
        // b = true
    }
}

void path_013_expected_5(int a, int b){
    while (a){
        if (b == 1){
            return;
        }
        if (b == 2){
            return;
        }
        if (b == 3){
            return;
        }
        a--;
    }
}

void path_014_expected_4(int a, int b, int c){
    if (a){
        return;
    } else {
        if (b){
            return;
        }  else {
            if (c){
                // c = true
            } else {
                // c = false
            }
        }
    }
}

void path_015_expected_3(int a, int b, int c){
    int i;
    for (i = 0; i < 10; i++){
        if (i > 5){
            return;
        }
    }
}

void path_016_expected_8(int a, int b){
    switch (a){
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
    if (b){
        // b = true
    }
}


void path_017_expected_5(int a, int b){
    switch (a){
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

void path_018_expected_7(int a, int b){
    switch (a){
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
    
    if (b){
        // b = true
    }
}

void path_019_expected_4(int a, int b){
    if (a > 10) {
        while (a){
            if (b == 1){
                break;
            }    
        }
    }
}

void path_020_expected_7(int a, int b){
    if (a > 10) {
        while (a){
            if (b == 1){
                continue;
            }
            if (b == 2){
                continue;
            }
            if (b == 3){
                continue;
            }

            if (b == 2){
                break;
            }    

            return;
        }
    }
}

void path_021_expected_7(int a, int b){
    while (b){
        switch (a){
            case 0:
                return;
            case 1:
                switch (a){
                    case 0:
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        if (b > 0){
            b--;
        }
    }
}

void path_022_expected_10(int a, int b){
    switch (a){
        case 0:
            if (b) break;
            break;
        case 1:
            if (b) break;
            break;
        default:
            break;
    }
    if (b > 0){
        b--;
    }
}

void path_023_expected_3(int a, int b, int c){
    switch (a){
        case 0:
            if (b) {
            }
            break;
    }
}

void path_024_expected_5(int a, int b, int c){
    switch (a){
        case 0:
            if (b){
            }
            if (c){
            }
            break;
        case 1:
        default:
            break;
    }
}

void path_025_expected_5(int a, int b, int c){
    switch (a){
        case 0: {
            if (b){
            }
            if (c){
            }
            break;
        }
        default:
            break;
    }
}

void path_026_expected_9(int a, int b, int c){
    switch (a){
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
    if (b){

    } else {

    }
    if (c){

    } else {

    }
}

void path_027_expected_24(int a, int b, int c){
    switch (a){
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
    if (b){

    } else {

    }
    if (c){

    } else {

    }
}

int main(int argc, char ** argv){
    return 0;
}
