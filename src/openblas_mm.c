#include<linux/types.h>
#include<librdtsc/rdtsc.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<cblas.h>
#include <time.h>
#include <stdio.h>
#include<sys/time.h>

//typedef u64 uint64_t;
//typedef s64 int64_t;

#ifndef M_SIZE
#define M 128
#define K 128
#define N 128
#endif

#define M M_SIZE
#define N N_SIZE
#define K K_SIZE
int main(){

	struct timeval start, end;
	double* A = (double*)malloc(M * K * sizeof(double));
	double* B = (double*)malloc(K * N * sizeof(double));
	double* C = (double*)calloc(M * N,  sizeof(double));
	
	__builtin___clear_cache;	

	srand(time(NULL));	

	if (!A || !B || !C) {
//        	printf(stderr, "Memory allocation failed\n");
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
	gettimeofday(&start, NULL);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, K, N, M, 1.0, A, M, B, N, 0.0, C, N); 
	gettimeofday(&end, NULL);

	long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
	printf("Elapsed time (us): %ld \n", (seconds * 1000000) + microseconds);

	return 0;
}

