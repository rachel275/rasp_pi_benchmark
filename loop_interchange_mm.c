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
void multiplyMatrix(int** m1, int** m2){
	
	int **result = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		result[i] = (int* )malloc (C2 * sizeof(int));
	}
	
	/** initialise matrix c**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			result[i][j] = 0;
		}
	}
	
	/**This loop order is independent and by switching the order of j and k, we have eliminate strided memory accesses so onlyu constant (a[i][k]) and sequential (c[i][j] and b[k][j]) memory access remain**/
	for (int i  = 0; i < R1; i++){
		for (int k = 0; k < R2; k++){
			for (int j = 0; j < C2; j++){
				result[i][j] = result[i][j] + m1[i][k] * m2[k][j];
			}
		}
	}
}

int main()
{
//	int m1[R1][C1] = {{1, 1}, {2, 2}};
	
//	int m2[R2][C2] = {{1, 1, 1}, {2, 2, 2}};
	
	__builtin___clear_cache;
	int **m1 = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		m1[i] = (int* )malloc (C1 * sizeof(int));
	}
	
	int **m2 = (int**)malloc(R2 * sizeof(int *));
	for (int j = 0; j < R1; j++){
		m2[j] = (int* )malloc (C2 * sizeof(int));
	}

	srand(time(NULL));
	for(int o = 0; o < R1; o++){
		for(int i = 0; i < C1; i++){
			m1[o][i] = rand();
		} 
	}

	for(int p = 0; p < R2; p++){
		for(int q = 0; q < C2; q++){
			m2[p][q] = rand();
		}
	}

	if (C1 != R2){
		printf("The number of columns in Matrix-1 must be equal to the number of rows in Matrix-2");
		exit(EXIT_FAILURE);
	}
	
	multiplyMatrix(m1, m2);
	
	return 0;
}
