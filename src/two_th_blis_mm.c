#include<stdlib.h>
#include<stdint.h>
#include<librdtsc/rdtsc.h>
#include<blis/blis.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>

#ifndef M_SIZE
#define M 128
#define K 128
#define N 128
#endif

#define M M_SIZE
#define N N_SIZE
#define K K_SIZE

int main(){

	//Initialize BLIS
	//bli_init();
	bli_thread_set_num_threads(2);
	bli_thread_set_ways(1, 1, 2, 1, 1);
	struct timeval start, end;
    
	double* A = (double*)malloc(M * K * sizeof(double));
	double* B = (double*)malloc(K * N * sizeof(double));
	double* C = (double*)calloc(M * N,  sizeof(double));

	__builtin___clear_cache;	

	srand(time(NULL));	

	if (!A || !B || !C) {
        	fprintf(stderr, "Memory allocation failed\n");
        	exit(EXIT_FAILURE);
    	}

    	// Input elements of A
    	//printf("Enter elements of matrix A (%lu x %lu):\n", m, k);
    	for (int i = 0; i < M * K; i++) {
        	A[i] = (double)rand() / RAND_MAX;
    	}

    	// Input elements of B
    	//printf("Enter elements of matrix B (%lu x %lu):\n", k, n);
    	for (int i = 0; i < K * N; i++) {
        	B[i] = (double)rand() / RAND_MAX;
    	}

	double alpha = 1.0, beta = 0.0;
//	obj_t A_blis, B_blis, C_blis;
//	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, K, A, 1, M, &A_blis);
//	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, K, N, B, 1, K, &B_blis);
//	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, N, C, 1, M, &C_blis);
	
	gettimeofday(&start, NULL);
	// Perform matrix multiplication using BLIS
//	bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);
	bli_dgemm(BLIS_NO_TRANSPOSE, BLIS_NO_TRANSPOSE, M, N, K, &alpha, A, 1, M, B, 1, K, &beta, C, 1, M);	
	gettimeofday(&end, NULL);
	/**	
	gettimeofday(&start, NULL);
	free(A);
	free(B);
	free(C);
	**/
	long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
	
	printf("Elapsed time (us): %ld \n", (seconds * 1000000) + microseconds);
	//bli_finalize();

	printf("GFLOPS: %f \n", (double) ((2 * pow(M,3) - pow(M,2)) / ((seconds * 1000000.0 + microseconds) / 1000000.0)) / 1000000000.0) ;
	return 0;
}
