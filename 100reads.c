#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    const int num_operations = 1000000;
    const int data_size = 1024;  // Size of data to read (1 KB)
    char *data = malloc(data_size * num_operations);

    // Initialize data with random bytes
    for (int i = 0; i < num_operations; i++) {
        data[i] = rand() % 256;
    }

    clock_t start_time = clock();

    for (int i = 0; i < num_operations; i++) {
        char value = data[i % num_operations];  // Simulate a read operation
        (void)value;  // To avoid unused variable warning
    }

    clock_t end_time = clock();
    free(data);

    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double total_data_MB = (num_operations * data_size) / (1024.0 * 1024.0);  // Convert to MB
    double throughput = total_data_MB / total_time;  // MB/s

    printf("Total Read Time: %f seconds\n", total_time);
    printf("Throughput: %f MB/s\n", throughput);
    return 0;
}
