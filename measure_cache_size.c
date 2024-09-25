#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE (1 << 22)  // Maximum size of the array (4M)

int main() {
    int *array;
    volatile int sum = 0;  // Volatile to prevent optimization
    clock_t start, end;

    // Test for various sizes of the array
    for (int size = (1 << 10); size <= MAX_SIZE; size <<= 1) {  // Start from 1K and double until 4M
	array = (int *)malloc(size * sizeof(int));  // Allocate memory for the array
        if (array == NULL) {
            perror("Failed to allocate memory");
            return EXIT_FAILURE;  // Exit if memory allocation fails
        }

        // Access the array to fill cache
        for (int i = 0; i < size; i++) {
            array[i] = i;  // Fill the array with values
        }

        // Measure latency
        start = clock();
        for (int i = 0; i < size; i++) {
            sum += array[i];  // Accessing array elements
        }
        end = clock();

        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;  // Calculate the elapsed time
        printf("Array size: %d, Time taken: %f seconds\n", size, time_taken);

        free(array);  // Free the allocated memory
    }

    return 0;
}
