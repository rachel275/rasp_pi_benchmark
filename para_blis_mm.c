#include<stdlib.h>
#include<blis/blis.h>
#include <time.h>

#ifndef MAT_SIZE
#define MAT_SIZE 500
#endif

#define M MAT_SIZE
#define N MAT_SIZE
#define K MAT_SIZE

int main() {
	// Initialize BLIS
	bli_init();

	// Set the number of threads for BLIS
	bli_thread_set_num_threads(4);
	//bli_thread_set_thread_impl(BLIS_OPENMP);
	bli_thread_set_ways(4, 1, 1, 1, 1);
    
	float* A = (float*)malloc(M * K * sizeof(float));
	float* B = (float*)malloc(K * N * sizeof(float));
	float* C = (float*)calloc(M * N, sizeof(float));

 	if (!A || !B || !C) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));

	// Initialize matrices A and B with random values
	for (int i = 0; i < M * K; i++) {
		A[i] = (float)rand() / RAND_MAX; // Random float between 0 and 1
	}

	for (int i = 0; i < K * N; i++) {
		B[i] = (float)rand() / RAND_MAX; // Random float between 0 and 1
	}
	
	float alpha = 1.0f, beta = 0.0f;
	obj_t A_blis, B_blis, C_blis;
	// Create BLIS objects for matrices
	bli_obj_create_with_attached_buffer(BLIS_FLOAT, M, K, A, 1, K, &A_blis);
	bli_obj_create_with_attached_buffer(BLIS_FLOAT, K, N, B, 1, N, &B_blis);
	bli_obj_create_with_attached_buffer(BLIS_FLOAT, M, N, C, 1, N, &C_blis);
	
	// Perform matrix multiplication using BLIS
	bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);

	// Free allocated memory
	free(A);
	free(B);
	free(C);

	// Finalize BLIS
	bli_finalize();

	return 0;
}
