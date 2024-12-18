#include<stdlib.h>
#include<cblas.h>
#include <time.h>

#ifndef MAT_SIZE
#define MAT_SIZE 500
#endif

#define M MAT_SIZE
#define N MAT_SIZE
#define K MAT_SIZE
#define C2 MAT_SIZE



int main(){

	
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
        	A[i] = rand();
    	}

    	// Input elements of B
    	//printf("Enter elements of matrix B (%lu x %lu):\n", k, n);
    	for (int i = 0; i < K * N; i++) {
        	B[i] = rand();
    	}
	
	//double alpha = 1.0, beta = 0.0;

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, K, N, M, 1.0, A, M, B, N, 0.0, C, N); 
	
	free(A);
	free(B);
	free(C);

	return 0;
}

