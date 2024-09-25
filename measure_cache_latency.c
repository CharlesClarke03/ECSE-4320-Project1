#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (1 << 30)  // Size of the array (1M)

int main() {
    int *array = (int *)malloc(SIZE * sizeof(int));
    volatile int sum = 0;  // Volatile to prevent optimization
    clock_t start, end;

    // Access the array to fill cache
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }

    // Measure latency
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];  // Accessing array elements
    }
    end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Size of Array: %d values\n", SIZE);
    printf("Time taken: %f seconds\n", time_taken);

    free(array);
    return 0;
}
