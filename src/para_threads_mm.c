#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <blis/blis.h>
#include <time.h>
#include <sched.h>

#ifndef M_SIZE
#define M_SIZE 128  // Matrix A rows
#define N_SIZE 128  // Matrix B columns
#define K_SIZE 128  // Matrix A columns and Matrix B rows
#endif

#define M M_SIZE
#define K K_SIZE
#define N N_SIZE
#define THREAD_COUNT 4  // Number of threads to create
#define NUM_TASKS 32   // Total number of matrix multiplication tasks

// Structure to hold the buffer of tasks for each thread
typedef struct {
    int start_task_id;  // First task ID for this thread
    int num_tasks;      // Number of tasks assigned to this thread
    int core_id;        // Core to pin the thread to
} thread_data_t;

void* matrix_thread_func(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int core_id = data->core_id;

    // Pin thread to a specific core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        fprintf(stderr, "Error pinning thread to core %d\n", core_id);
        pthread_exit(NULL);
    }

    // Initialize BLIS for this thread
    bli_init();

    // Execute tasks assigned to this thread
    for (int task_id = data->start_task_id; task_id < data->start_task_id + data->num_tasks; task_id++) {
        // Allocate matrices A, B, and C
        double* A = (double*)malloc(M * K * sizeof(double));
        double* B = (double*)malloc(K * N * sizeof(double));
        double* C = (double*)calloc(M * N, sizeof(double));  // Zero-initialized

        if (!A || !B || !C) {
            fprintf(stderr, "Memory allocation failed for task %d\n", task_id);
            free(A); free(B); free(C);
            continue;
        }

        // Seed the random number generator for unique values
        srand(time(NULL) + task_id);

        // Initialize matrices A and B
        for (int i = 0; i < M * K; i++) {
            A[i] = (double)rand() / RAND_MAX;
        }
        for (int i = 0; i < K * N; i++) {
            B[i] = (double)rand() / RAND_MAX;
        }

        // Create BLIS objects
        obj_t A_blis, B_blis, C_blis;
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, K, A, 1, M, &A_blis);
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, K, N, B, 1, K, &B_blis);
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, N, C, 1, M, &C_blis);

        // Perform matrix multiplication
        bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);

        // Free memory
        free(A);
        free(B);
        free(C);
    }

    // Finalize BLIS for this thread
//    bli_finalize();
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    thread_data_t thread_data[THREAD_COUNT];

    // Calculate the number of tasks per thread
    int tasks_per_thread = NUM_TASKS / THREAD_COUNT;
    int remaining_tasks = NUM_TASKS % THREAD_COUNT;  // Handle cases where tasks are not evenly divisible

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].start_task_id = i * tasks_per_thread;
        thread_data[i].num_tasks = tasks_per_thread + (i < remaining_tasks ? 1 : 0);  // Distribute extra tasks evenly
        thread_data[i].core_id = i % THREAD_COUNT;

        // Create the thread
        if (pthread_create(&threads[i], NULL, matrix_thread_func, &thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All tasks completed!\n");
    return 0;
}

