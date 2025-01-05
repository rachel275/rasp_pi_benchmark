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
#define THREAD_COUNT 4  // Number of threads in the pool
#define TASK_COUNT 8    // Number of tasks to perform

typedef struct {
    int task_id;
} task_data_t;

// Task queue and control variables
task_data_t task_queue[TASK_COUNT];
int task_index = 0;  // Next task to be picked
pthread_mutex_t queue_mutex;
pthread_cond_t queue_cond;

void* matrix_thread_func(void* arg) {
    int core_id = *(int*)arg;

    // Pin thread to a specific core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        fprintf(stderr, "Error pinning thread to core %d\n", core_id);
        pthread_exit(NULL);
    }

    bli_init();  // Initialize BLIS for this thread

    while (1) {
        pthread_mutex_lock(&queue_mutex);

        if (task_index >= TASK_COUNT) {
            pthread_mutex_unlock(&queue_mutex);
            break;  // All tasks have been assigned
        }

        // Fetch the next task
        int task_id = task_queue[task_index++].task_id;
        pthread_mutex_unlock(&queue_mutex);

        // Allocate matrices A, B, and C for this task
        double* A = (double*)malloc(M * K * sizeof(double));
        double* B = (double*)malloc(K * N * sizeof(double));
        double* C = (double*)calloc(M * N, sizeof(double));  // Zero-initialized

        if (!A || !B || !C) {
            fprintf(stderr, "Memory allocation failed for task %d\n", task_id);
            pthread_exit(NULL);
        }

        srand(time(NULL) + task_id);
        for (int i = 0; i < M * K; i++) A[i] = (double)rand() / RAND_MAX;
        for (int i = 0; i < K * N; i++) B[i] = (double)rand() / RAND_MAX;

        obj_t A_blis, B_blis, C_blis;
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, K, A, 1, M, &A_blis);
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, K, N, B, 1, K, &B_blis);
        bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, N, C, 1, M, &C_blis);

        // Perform matrix multiplication
        bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);

//        printf("Core %d completed task %d\n", core_id, task_id);
/**
        // Free memory
        bli_obj_free(&A_blis);
        bli_obj_free(&B_blis);
        bli_obj_free(&C_blis);
        free(A);
        free(B);
        free(C);
**/
    }

//    bli_finalize();  // Finalize BLIS for this thread
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int core_ids[THREAD_COUNT];
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&queue_cond, NULL);

    // Create 8 tasks in the queue
    for (int i = 0; i < TASK_COUNT; i++) {
        task_queue[i].task_id = i;
    }

    // Create 4 threads for the thread pool
    for (int i = 0; i < THREAD_COUNT; i++) {
        core_ids[i] = i;
        if (pthread_create(&threads[i], NULL, matrix_thread_func, &core_ids[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads after completion
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);

 //   printf("All tasks completed successfully.\n");
    return 0;
}

