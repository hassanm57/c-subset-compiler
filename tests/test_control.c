/* test_control.c - if/else, while, for, break, continue */

int main() {
    int i;
    int sum;
    sum = 0;

    /* For loop: sum 1..10 */
    for (i = 1; i <= 10; i++) {
        sum = sum + i;
    }
    printf("Sum 1-10 = %d\n", sum);

    /* While with break */
    int n;
    n = 100;
    while (n > 0) {
        if (n % 7 == 0) {
            break;
        }
        n = n - 1;
    }
    printf("First multiple of 7 <= 100: %d\n", n);

    /* Nested if/else */
    int score;
    score = 75;
    if (score >= 90) {
        printf("Grade: A\n");
    } else if (score >= 80) {
        printf("Grade: B\n");
    } else if (score >= 70) {
        printf("Grade: C\n");
    } else {
        printf("Grade: F\n");
    }

    /* For with continue: print evens only */
    for (i = 1; i <= 10; i++) {
        if (i % 2 != 0) {
            continue;
        }
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}
