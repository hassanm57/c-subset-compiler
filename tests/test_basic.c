/* test_basic.c - Basic arithmetic, variables, expressions */

int x;
int y;
float z;

int add(int a, int b) {
    return a + b;
}

int main() {
    x = 10;
    y = 20;
    z = 3.14;

    int sum;
    sum = add(x, y);

    int result;
    result = (x + y) * 2 - 5;

    /* Constant folding candidate: 2 * 3 + 4 */
    int constant;
    constant = 2 * 3 + 4;

    /* Algebraic simplification: x * 1, x + 0 */
    int identity;
    identity = x * 1 + 0;

    float area;
    area = z * z;

    printf("sum = %d\n", sum);
    printf("result = %d\n", result);
    printf("constant = %d\n", constant);
    printf("identity = %d\n", identity);
    printf("area = %f\n", area);

    return 0;
}
