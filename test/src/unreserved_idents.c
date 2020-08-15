/* Test code for CCSM ( http://dev.brightsilence.com/ccsm/ )
 *
 * (c) 2016 John Bailey
 */

#define VAR a = b

char a = 'a';
char b = 'a';
char c = 'a';

void func_a(void) {
}

void func_b(void) {
    VAR = 'b';
}

void func_c(void) {
    a = b = 'a';
}

void func_d(void) {
    unsigned j;

    VAR = c = j;
}

void func_e(void) {
    VAR;
}

char d = 'c';
