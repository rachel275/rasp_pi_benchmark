#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAT_SIZE
#define MAT_SIZE 500
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE

void multiplyMatrix(double** m1, double** m2){
	
	double **result = (double**)malloc(R1 * sizeof(double *));
	for (int i = 0; i < R1; i++){
		result[i] = (double* )malloc (C2 * sizeof(double));
	}
	
	/** initialise matrix c**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			result[i][j] = 0.0;
		}
	}
	
	/**This loop order is independent and by switching the order of j and k, we have eliminate strided memory accesses so only constant (a[i][k]) and sequential (c[i][j] and b[k][j]) memory access remain**/
	for (int i  = 0; i < R1; i++){
		for (int k = 0; k < R2; k++){
			for (int j = 0; j < C2; j++){
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}

int main()
{
	__builtin___clear_cache;
	double **m1 = (double**)malloc(R1 * sizeof(double *));
	for (int i = 0; i < R1; i++){
		m1[i] = (double* )malloc (C1 * sizeof(double));
	}
	
	double **m2 = (double**)malloc(R2 * sizeof(double *));
	for (int j = 0; j < R1; j++){
		m2[j] = (double* )malloc (C2 * sizeof(double));
	}

	srand(time(NULL));
	for(int o = 0; o < R1; o++){
		for(int i = 0; i < C1; i++){
			m1[o][i] = ((double)rand() / RAND_MAX) * 100.0;
		} 
	}

	for(int p = 0; p < R2; p++){
		for(int q = 0; q < C2; q++){
			m2[p][q] = ((double)rand() / RAND_MAX) * 100.0;
		}
	}

	if (C1 != R2){
		printf("The number of columns in Matrix-1 must be equal to the number of rows in Matrix-2\n");
		exit(EXIT_FAILURE);
	}
	
	multiplyMatrix(m1, m2);
	
	return 0;
}
