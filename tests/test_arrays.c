/* test_arrays.c - Array support: declaration, access, initializers, functions */

/* Bubble sort on array */
void bubble_sort(int arr[], int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp     = arr[j];
                arr[j]   = arr[j + 1];
                arr[j+1] = temp;
            }
        }
    }
}

/* Sum of array */
int array_sum(int arr[], int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

/* Linear search */
int linear_search(int arr[], int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int main() {
    /* Array with initializer */
    int data[5] = {64, 34, 25, 12, 22};
    int n;
    n = 5;

    printf("Before sort: ");
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    bubble_sort(data, n);

    printf("After sort:  ");
    for (i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    printf("Sum = %d\n", array_sum(data, n));

    /* Search for value 25 */
    int idx;
    idx = linear_search(data, n, 25);
    printf("Search 25: index = %d\n", idx);

    /* Float array */
    float scores[4] = {9.5, 8.0, 7.5, 6.0};
    float total;
    total = 0.0;
    for (i = 0; i < 4; i++) {
        total = total + scores[i];
    }
    printf("Average score: %f\n", total / 4);

    /* Multi-element array manipulation */
    int matrix[9];
    for (i = 0; i < 9; i++) {
        matrix[i] = i * i;
    }
    printf("Squares: ");
    for (i = 0; i < 9; i++) {
        printf("%d ", matrix[i]);
    }
    printf("\n");

    return 0;
}
