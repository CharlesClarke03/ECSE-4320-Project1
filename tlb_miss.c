#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#define BILLION 1000000000L

// Function to perform sequential memory access (lightweight multiplication)
void sequential_access(int *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        array[i] *= 2;
    }
}

// Function to perform random memory access (lightweight multiplication)
void random_access(int *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        size_t index = rand() % size;
        array[index] *= 2;
    }
}

// Function to get the time difference in seconds between two timespec structs
double time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <memory size in MB>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t memory_size_mb = atoi(argv[1]);
    size_t array_size = (memory_size_mb * (1 << 20)) / sizeof(int);

    srand(time(NULL));  // Seed for random number generation

    // Allocate memory (using malloc for now; mmap with huge pages can be used)
    int *array = malloc(array_size * sizeof(int));
    if (array == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    // Initialize the array with random data
    for (size_t i = 0; i < array_size; i++) {
        array[i] = rand();
    }

    // Measure time for sequential access
    struct timespec start_seq, end_seq;
    clock_gettime(CLOCK_MONOTONIC, &start_seq);
    sequential_access(array, array_size);
    clock_gettime(CLOCK_MONOTONIC, &end_seq);

    // Measure time for random access
    struct timespec start_rand, end_rand;
    clock_gettime(CLOCK_MONOTONIC, &start_rand);
    random_access(array, array_size);
    clock_gettime(CLOCK_MONOTONIC, &end_rand);

    // Calculate and print execution times
    double time_seq = time_diff(start_seq, end_seq);
    double time_rand = time_diff(start_rand, end_rand);

    // Calculate Throughput and Latency
    double throughput_seq = array_size / time_seq;  // accesses per second
    double throughput_rand = array_size / time_rand;  // accesses per second

    double latency_seq = time_seq / array_size;  // time per access in seconds
    double latency_rand = time_rand / array_size;  // time per access in seconds

    printf("Memory size: %zu MB\n", memory_size_mb);
    printf("Sequential access time: %.6f seconds\n", time_seq);
    printf("Random access time: %.6f seconds\n", time_rand);
    
    printf("Throughput (Sequential): %.2f million accesses per second\n", throughput_seq / 1e6);
    printf("Throughput (Random): %.2f million accesses per second\n", throughput_rand / 1e6);
    
    printf("Latency (Sequential): %.6f ns per access\n", latency_seq * 1e9);
    printf("Latency (Random): %.6f ns per access\n", latency_rand * 1e9);

    // Free allocated memory
    free(array);

    printf("Reminder: Run with 'perf stat -e dTLB-load-misses,dTLB-store-misses,iTLB-load-misses' for TLB miss data.\n\n");

    return 0;
}
