/*  demo has--- variables, arithmetic, control flow,
          functions, arrays, and optimization */

int result;
int count;

/* Function: returns factorial of n recursively */
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int sub;
    sub = n - 1;
    int rec;
    rec = factorial(sub);
    return n * rec;
}

/* Function: sums elements of an array */
int sum_array(int arr[], int size) {
    int total;
    int i;
    total = 0;
    i = 0;
    while (i < size) {
        total = total + arr[i];
        i = i + 1;
    }
    return total;
}

/* function: returns larger of two values */
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int main() {

    /* Optimizer demo: constant folding + algebraic simplification --- */
    int x;
    int y;
    int z;
    x = 4 * 5 + 10;       /* optimizer folds this to 30 at compile time  */
    y = x * 1 + 0;        /* optimizer simplifies: x*1 -> x, x+0 -> x    */
    z = x + y;

    printf("x = %d\n", x);
    printf("y = %d\n", y);
    printf("z = %d\n", z);

    /* control flow demo: if / else if / else --- */
    int score;
    score = 73;

    if (score >= 90) {
        printf("Grade: A\n");
    } else {
        if (score >= 80) {
            printf("Grade: B\n");
        } else {
            if (score >= 70) {
                printf("Grade: C\n");
            } else {
                printf("Grade: F\n");
            }
        }
    }

    /* loop demo: for loop with accumulator --- */
    int i;
    int running_total;
    running_total = 0;
    for (i = 1; i <= 5; i = i + 1) {
        running_total = running_total + i;
        printf("Step %d: total = %d\n", i, running_total);
    }

    /* array demo --- */
    int nums[5];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    nums[4] = 50;

    int arr_sum;
    arr_sum = sum_array(nums, 5);
    printf("Array sum = %d\n", arr_sum);

    /* recursion demo: factorial --- */
    int f;
    f = factorial(6);
    printf("6! = %d\n", f);

    /* --- function call + comparison demo --- */
    int bigger;
    bigger = max(f, arr_sum);
    printf("max(6!, arr_sum) = %d\n", bigger);

    /* --- While loop with break --- */
    int n;
    n = 1;
    while (n <= 100) {
        if (n * n > 50) {
            break;
        }
        n = n + 1;
    }
    printf("First n where n*n > 50: %d\n", n);

    return 0;
}