#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  // For timing

double get_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec + time.tv_usec * 1e-6;
}

void multiply_arrays(double *a, double *b, int n, int stride) {
    double result = 0.0;
    for (int i = 0; i < n; i += stride) {
        result += a[i] * b[i];
    }
}

int main() {
    int n = 400000000;  // Number of iterations
    double *a = (double*) malloc(n * sizeof(double));
    double *b = (double*) malloc(n * sizeof(double));

    // Initialize arrays
    for (int i = 0; i < n; i++) {
        a[i] = i * 0.5;
        b[i] = i * 2.5;
    }

    int stride_sizes[] = {1, 2, 4, 8, 16, 32, 64};  // Array of stride sizes
    int num_strides = sizeof(stride_sizes) / sizeof(stride_sizes[0]);

    // Measure execution time and throughput for different stride sizes
    for (int j = 0; j < num_strides; j++) {
        int stride = stride_sizes[j];

        double start = get_time();
        multiply_arrays(a, b, n, stride);
        double end = get_time();

        // Calculate total operations
        long total_operations = n / stride;  // Number of multiplications performed
        double execution_time = end - start; // Execution time in seconds

        // Calculate throughput
        double throughput = total_operations / execution_time; // Operations per second

        printf("Stride: %d, Time: %f seconds, Throughput: %f OPS\n", stride, execution_time, throughput);
    }

    free(a);
    free(b);
    return 0;
}
