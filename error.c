/* improvement_test.c
   Tests all 7 lexer improvements — IMP-1 through IMP-7
   Each section is labeled so you know which error maps to which improvement */

int main() {

    /* IMP-1: Invalid identifier — starts with a digit */
    int x;

    /* IMP-2: Integer literal overflow — exceeds INT_MAX (2147483647) */
    int big;
    big = 999999999999999999999999999;

    /* IMP-3: Unterminated string literal — missing closing quote */
    printf("hello world");

    /* IMP-4: Unterminated block comment — missing closing */
    /* this comment never ends...

    /* IMP-5: Unterminated char literal — missing closing quote */
    char c;
    c = 'a;

    /* IMP-6: Multi-character char literal */
    char d;
    d = 'ab';

    return 0;
}