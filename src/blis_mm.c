#include<stdlib.h>
#include<blis/blis.h>
#include <time.h>



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
	bli_init();

	bli_thread_set_num_threads(1);
	//bli_thread_set_thread_impl(BLIS_OPENMP);
	bli_thread_set_ways(1, 1, 1, 1, 1);
    
	//dim_t m , k, n;

	double* A = (double*)malloc(M * K * sizeof(double));
	double* B = (double*)malloc(K * N * sizeof(double));
	double* C = (double*)calloc(M * N,  sizeof(double));
//	printf("reached here");

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

	obj_t A_blis, B_blis, C_blis;
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, K, A, 1, M, &A_blis);
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, K, N, B, 1, K, &B_blis);
	bli_obj_create_with_attached_buffer(BLIS_DOUBLE, M, N, C, 1, M, &C_blis);
	
//	printf("now here");

//	bli_printm("A_blis: ", &A_blis, "%4.1f", "");

	bli_gemm(&BLIS_ONE, &A_blis, &B_blis, &BLIS_ZERO, &C_blis);
/**	
	bli_obj_free(&A_blis);
	bli_obj_free(&B_blis);
	bli_obj_free(&C_blis);
**/
	free(A);
	free(B);
	free(C);

	bli_finalize();

	return 0;
}
