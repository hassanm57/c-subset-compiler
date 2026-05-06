/* test_functions.c - Functions, recursion, parameters */

/* Recursive factorial */
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

/* Fibonacci (iterative) */
int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    int a;
    int b;
    int c;
    int i;

    a = 0;
    b = 1;
    i = 2;

    while (i <= n) {
        c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

/* GCD using Euclidean algorithm */
int gcd(int a, int b) {
    while (b != 0) {
        int temp;
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Max of two */
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

/* Power function */
float power(float base, int exp) {
    float result;
    result = 1.0;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

int main() {
    printf("5! = %d\n", factorial(5));
    printf("10! = %d\n", factorial(10));

    printf("fib(10) = %d\n", fibonacci(10));
    printf("fib(15) = %d\n", fibonacci(15));

    printf("gcd(48, 18) = %d\n", gcd(48, 18));
    printf("max(42, 17) = %d\n", max(42, 17));

    printf("2^10 = %f\n", power(2.0, 10));

    return 0;
}
