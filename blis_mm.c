#include<stdlib.h>
#include<blis/blis.h>


#include <time.h>
//#include <sys/cachectl.h>

#ifndef MAT_SIZE
#define MAT_SIZE 500
#endif

#define M MAT_SIZE
#define N MAT_SIZE
#define K MAT_SIZE
#define C2 MAT_SIZE

/**now to initialise this in a c file**/ 


int main(){

	//int m , k, n;

	float* A = (float*)malloc(M * K * sizeof(float));
	float* B = (float*)malloc(K * N * sizeof(float));
	float* C = (float*)malloc(M * N * sizeof(float));

	

	__builtin___clear_cache;	
	//int **m1 = (int**)malloc(R1 * sizeof(int *));
	//for (int i = 0; i < R1; i++){
	//	m1[i] = (int* )malloc (C1 * sizeof(int));
	//}

	//int **m2 = (int**)malloc(R2 * sizeof(int *));
	//for (int j = 0; j < R2; j++){
	//	m2[j] = (int* )malloc (C2 * sizeof(int));
	//}

	srand(time(NULL));	
	
	for(int o = 0; o < M; o++){
		for(int n = 0; n < K; n++){
			A[o * K + n] = rand();
		} 
	}

	for(int p = 0; p < K; p++){
		for(int q = 0; q < N; q++){
			B[p * N + q] = rand();
		}
	}


	for(int p = 0; p < M; p++){
		for(int q = 0; q < N; q++){
			C[p * N + q] = 0;
		}
	}

	float alpha = 0.1f, beta = 0.0f;
	
	bli_sgemm(BLIS_NO_TRANSPOSE, BLIS_NO_TRANSPOSE, BLIS_NO_TRANSPOSE,M, N, K, &alpha, A, K, B, N, &beta, C,N);

	free(A);
	free(B);
	free(C);

	return 0;
}

