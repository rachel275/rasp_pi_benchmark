/** We have two matrices of size R1, C1, R2, C2 - turn these into matrices A11[R1/2][R1/2], A12, A21, A22, **/
/**
 *
 * M1 = (a11 + a22) * (b11 + b22)
 * M2 = (a21 + a22) * b11
 * M3 = a11 * (b12 - b22)
 * M4 = a22 * (b21 - b11)
 * M5 = (a11 + a12) * b22
 * M6 = (a12 - a11) * (b11 + b12)
 * M7 = (a12 - a22) * (b21 + b22)
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <sys/cachectl.h>

#ifndef MAT_SIZE
#define MAT_SIZE 1024
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE

int** add(int**temp, int** a, int** b){

	//int temp[R1/2][R1/2];
	for(int i = 0; i < R1/2; i++){
		for(int j = 0; j < R1/2; j++){
			temp[i][j] = a[i][j] + b[i][j];
		}
	}
	return temp;
}

int** sub(int** temp, int** a, int** b){
	

	//int temp[R1/2][R1/2];
	for(int i = 0; i < R1/2; i++){
		for(int j = 0; j < R1/2; j++){
			temp[i][j] = a[i][j] - b[i][j];
		}
	}
	return temp;
}


int** mult(int** result, int** a, int** b){
	

//	int **result = (int**)malloc(R1 * sizeof(int *));
//	for (int i = 0; i < R1; i++){
//		result[i] = (int* )malloc (C1 * sizeof(int));
//	}
	
	/** simple loop for matrix multiplication**/
	for (int i = 0; i < R1/2; i++) {
		for (int j = 0; j < C2/2; j++){
			result[i][j] = 0;
			for (int k = 0; k < R2/2; k++){
				result[i][j] = result[i][j] + a[i][k] * b[k][j];
			}
//			printf("%d\t", result[i][j]);
		}
	}
	return result;
}


void Strassen(int** a, int** b){

	int **a11 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		a11[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			a11[i][k] = a[i][k];
		}
	}


	int **a12 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		a12[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			a12[i][k] = a[i][k + (R1/2)];
		}
	}


	int **a21 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		a21[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			a21[i][k] = a[i + (R1/2)][k];
		}
	}


	int **a22 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		a22[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			a22[i][k] = a[i + (R1/2)][k + (R1/2)];
		}
	}


	int **b11 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		b11[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			b11[i][k] = b[i][k];
		}
	}


	int **b12 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		b12[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			b12[i][k] = b[i][k + (R1/2)];
		}
	}


	int **b21 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		b21[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			b21[i][k] = b[i + (R1/2)][k];
		}
	}


	int **b22 = (int**)malloc((R1/2) * sizeof(int *));
	for (int i = 0; i < (R1/2); i++){
		b22[i] = (int* )malloc ((C1/2) * sizeof(int));
		for (int k = 0; k < (R1/2); k++){
			b22[i][k] = b[i + (R1/2)][k + (R1/2)];
		}
	}


	/**
 	*
 	* M1 = (a11 + a22) * (b11 + b22)
 	* M2 = (a21 + a22) * b11
 	* M3 = a11 * (b12 - b22)
 	* M4 = a22 * (b21 - b11)
 	* M5 = (a11 + a12) * b22
 	* M6 = (a12 - a11) * (b11 + b12)
 	* M7 = (a12 - a22) * (b21 + b22)
 	**/
/**
	int m1[R1/2][R1/2];	
	int m2[R1/2][R1/2];
	int m3[R1/2][R1/2];
	int m4[R1/2][R1/2];
	int m5[R1/2][R1/2];
	int m6[R1/2][R1/2];
	int m7[R1/2][R1/2];
	int temp1[R1/2][R1/2];
	int temp2[R1/2][R1/2];

**/
	int **m1 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m1[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **m2 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m2[i] = (int* )malloc (C1/2 * sizeof(int));
	}


	int **m3 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m3[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **m4 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m4[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **m5 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m5[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **m6 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m6[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **m7 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		m7[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **temp1 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		temp1[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	int **temp2 = (int**)malloc(R1/2 * sizeof(int *));
	for (int i = 0; i < R1/2; i++){
		temp2[i] = (int* )malloc (C1/2 * sizeof(int));
	}

	//initialise the matrices so it doesn't shout at me
	m1 = mult(m1, add(temp1, a11, a22), add(temp2, b11, b22));
	m2 = mult(m2, add(temp1, a21, a22), b11);
	m3 = mult(m3, a11, add(temp1, b12, b22));
	m4 = mult(m4, a22, sub(temp1, b21, b11));
	m5 = mult(m5, add(temp1, a11, a12), add(temp2, b11, b12));
	m6 = mult(m6, sub(temp1, a12, a11), add(temp2, b11, b12));
	m7 = mult(m7, sub(temp1, a12, a22), add(temp2, b21, b22));

}


int main()
{
		
	int **a1 = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		a1[i] = (int* )malloc (C1 * sizeof(int));
	}

	int **b1 = (int**)malloc(R2 * sizeof(int *));
	for (int j = 0; j < R2; j++){
		b1[j] = (int* )malloc (C2 * sizeof(int));
	}

	srand(time(NULL));	
	
	for(int o = 0; o < R1; o++){
		for(int n = 0; n < C1; n++){
			a1[o][n] = rand();
		} 
	}

	for(int p = 0; p < R2; p++){
		for(int q = 0; q < C2; q++){
			b1[p][q] = rand();
		}
	}
	
	if (C1 != R2){
		printf("The number of columns in Matrix-1 must be equal to the number of rows in Matrix-2");
		exit(EXIT_FAILURE);
	}
	
//	cacheflush();
	__builtin___clear_cache;
	Strassen(a1, b1);
	
	return 0;
}
