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
#define THREAD_COUNT 4
#define NUM_TASKS 8
typedef struct {
    int thread_id;
    int core_id;  // Core to pin the thread
} thread_data_t;

// Function for matrix creation, initialization, and multiplication
void* matrix_thread_func(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int thread_id = data->thread_id;
    int core_id = data->core_id;

    // Pin thread to specific core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        fprintf(stderr, "Error pinning thread %d to core %d\n", thread_id, core_id);
        pthread_exit(NULL);
    }

    // Initialize BLIS for this thread
    bli_init();

    // Allocate matrices A, B, and C
    double* A = (double*)malloc(M * K * sizeof(double));
    double* B = (double*)malloc(K * N * sizeof(double));
    double* C = (double*)calloc(M * N, sizeof(double));  // Zero-initialized

    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation failed for thread %d\n", thread_id);
        pthread_exit(NULL);
    }

    // Seed the random number generator for unique values
    srand(time(NULL) + thread_id);

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

    // Perform matrix multiplication C = alpha * A * B + beta * C
//    printf("Thread %d (core %d): Performing matrix multiplication...\n", thread_id, core_id);
    bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);

    //bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);
    //printf("Thread %d (core %d): Matrix multiplication completed.\n", thread_id, core_id);
/**
    // Free memory
    bli_obj_free(&A_blis);
    bli_obj_free(&B_blis);
    bli_obj_free(&C_blis);
    free(A);
    free(B);
    free(C);

    // Finalize BLIS for this thread
    bli_finalize();
**/
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    thread_data_t thread_data[THREAD_COUNT];

    int num_cores = 4;//sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores < THREAD_COUNT) {
        fprintf(stderr, "Warning: Only %d cores available. Adjusting thread count.\n", num_cores);
        return EXIT_FAILURE;
    }
    int tasks_per_thread = NUM_TASKS / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        for (int j = 0; j < tasks_per_thread; j++) {
            // int task_id = i * tasks_per_thread + j;
            //thread_data_t* task_data = (thread_data_t*)malloc(sizeof(thread_data_t));
            //task_data->task_id = task_id;

            thread_data[i].thread_id = i;
            thread_data[i].core_id = i;  // Pin each thread to a unique core
            // Create a thread for each task
            if (pthread_create(&threads[i], NULL, matrix_thread_func, &thread_data[i]) != 0) {
                fprintf(stderr, "Error creating thread for task %d\n", i);
                exit(EXIT_FAILURE);
            }
            pthread_join(threads[i], NULL);  // Join after task is done
        }
    }

//    printf("All tasks completed!\n");
    return 0;
}

