#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <sys/cachectl.h>

#ifndef MAT_SIZE
#define MAT_SIZE 1000
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE

/**now to initialise this in a c file**/ 
void multiplyMatrix(int** m1, int** m2){
	

	int **result = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		result[i] = (int* )malloc (C1 * sizeof(int));
	}
	
	/** simple loop for matrix multiplication**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			result[i][j] = 0;
			for (int k = 0; k < R2; k++){
				result[i][j] = result[i][j] + m1[i][k] * m2[k][j];
			}
//			printf("%d\t", result[i][j]);
			
		}
		
//		printf("\n");
	}
}

int main()
{
	//int m1[R1][C1] = {{1, 1, 5, 5, 7}, {1, 2, 7, 435, 5}, {45, 56, 21, 6, 7}, {56, 6, 45, 3, 6}, {2, 4, 5, 4, 2}};
	
	//int m2[R2][C2] = {{1, 1, 1, 3, 5, 345, 6774}, {2, 2, 2, 356, 34, 56, 4}, {34, 3, 6, 5, 45, 56, 3}, {45, 2345, 6, 2, 4, 45, 4}, {34, 456, 678, 3, 2, 5, 65}};
	
	//int m1[R1][C1];
	//int m2[R2][C2];
	
	__builtin___clear_cache;	
	int **m1 = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		m1[i] = (int* )malloc (C1 * sizeof(int));
	}

	int **m2 = (int**)malloc(R2 * sizeof(int *));
	for (int j = 0; j < R2; j++){
		m2[j] = (int* )malloc (C2 * sizeof(int));
	}

	srand(time(NULL));	
	
	for(int o = 0; o < R1; o++){
		for(int n = 0; n < C1; n++){
			m1[o][n] = rand();
	//		printf(" %i", m1[o][n]);
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
	
//	cacheflush();
//	__builtin___clear_cache;
	multiplyMatrix(m1, m2);
	
	return 0;
}
