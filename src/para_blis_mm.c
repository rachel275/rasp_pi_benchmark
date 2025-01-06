#include<stdlib.h>
#include<blis.h>
#include <time.h>

#ifndef M_SIZE
#define M 128
#define K 128
#define N 128
#endif

#define M M_SIZE
#define N N_SIZE
#define K K_SIZE

int main() {
	// Initialize BLIS
	bli_init();

	// Set the number of threads for BLIS
	bli_thread_set_num_threads(4);
	//bli_thread_set_thread_impl(BLIS_OPENMP);
	bli_thread_set_ways(1, 1, 4, 1, 1);
    
	double* A = (double*)malloc(M * K * sizeof(double));
	double* B = (double*)malloc(K * N * sizeof(double));
	double* C = (double*)calloc(M * N, sizeof(double));

 	if (!A || !B || !C) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));

	// Initialize matrices A and B with random values
	for (int i = 0; i < M * K; i++) {
		A[i] = (double)rand() / RAND_MAX; // Random double between 0 and 1
	}

	for (int i = 0; i < K * N; i++) {
		B[i] = (double)rand() / RAND_MAX; // Random double between 0 and 1
	}
	
	double alpha = 1.0, beta = 0.0;
	obj_t A_blis, B_blis, C_blis;
	// Create BLIS objects for matrices
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, K, A, 1, M, &A_blis);
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, K, N, B, 1, K, &B_blis);
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, N, C, 1, M, &C_blis);
	
	// Perform matrix multiplication using BLIS
	bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);
/**
	// Free allocated memory
	free(A);
	free(B);
	free(C);

	// Finalize BLIS
	bli_finalize();
**/
	return 0;
}
