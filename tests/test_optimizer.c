/* test_optimizer.c
 * Demonstrates all optimizer passes:
 *   - Constant folding
 *   - Algebraic simplification
 *   - Dead code elimination
 *   - Constant propagation
 *   - Copy propagation
 */

int main() {
    /* Constant folding: 2*3+4 should become 10 */
    int a;
    a = 2 * 3 + 4;

    /* Algebraic simplification: x*1 → x,  x+0 → x */
    int b;
    b = a * 1 + 0;

    /* Constant propagation: c = 5 propagated forward */
    int c;
    c = 5;
    int d;
    d = c + 3;   /* becomes d = 5 + 3 → d = 8 */

    /* Dead code elimination: intermediate temp never used externally */
    int e;
    int f;
    e = 100;
    f = e * 2;   /* if f is not used, this becomes dead */

    /* Copy propagation: g = b, then use g → use b directly */
    int g;
    g = b;
    int h;
    h = g + 1;

    /* Compound: fully foldable expression */
    int result;
    result = (10 + 5) * 2 - 3;   /* = 27, fully folded */

    /* Division folding */
    int div_test;
    div_test = 20 / 4;   /* = 5 */

    /* Relational folding */
    int rel;
    rel = 3 > 2;   /* = 1 */

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("d = %d\n", d);
    printf("h = %d\n", h);
    printf("result = %d\n", result);
    printf("div_test = %d\n", div_test);
    printf("rel = %d\n", rel);

    return 0;
}
