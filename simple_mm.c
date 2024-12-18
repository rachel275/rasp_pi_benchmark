#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAT_SIZE
#define MAT_SIZE 1000
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE

/**now to initialise this in a c file**/ 
void multiplyMatrix(double** m1, double** m2){
	
	// Allocate memory for the result matrix
	double **result = (double**)malloc(R1 * sizeof(double *));
	for (int i = 0; i < R1; i++){
		result[i] = (double* )malloc (C2 * sizeof(double));
	}
	
	/** simple loop for matrix multiplication**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			result[i][j] = 0.0;
			for (int k = 0; k < R2; k++){
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}

    // Free the memory for the result matrix
    for (int i = 0; i < R1; i++){
        free(result[i]);
    }
    free(result);
}

int main()
{
	// Allocate memory for the first matrix
	double **m1 = (double**)malloc(R1 * sizeof(double *));
	for (int i = 0; i < R1; i++){
		m1[i] = (double* )malloc (C1 * sizeof(double));
	}

	// Allocate memory for the second matrix
	double **m2 = (double**)malloc(R2 * sizeof(double *));
	for (int j = 0; j < R2; j++){
		m2[j] = (double* )malloc (C2 * sizeof(double));
	}

	srand(time(NULL));	
	
	// Populate the first matrix with random double values
	for(int o = 0; o < R1; o++){
		for(int n = 0; n < C1; n++){
			m1[o][n] = ((double)rand() / RAND_MAX) * 100.0; // random double between 0 and 100
		} 
	}

	// Populate the second matrix with random double values
	for(int p = 0; p < R2; p++){
		for(int q = 0; q < C2; q++){
			m2[p][q] = ((double)rand() / RAND_MAX) * 100.0; // random double between 0 and 100
		}
	}

	if (C1 != R2){
		printf("The number of columns in Matrix-1 must be equal to the number of rows in Matrix-2\n");
		// Free allocated memory before exit
		for (int i = 0; i < R1; i++){
			free(m1[i]);
		}
		free(m1);
		
		for (int j = 0; j < R2; j++){
			free(m2[j]);
		}
		free(m2);
		exit(EXIT_FAILURE);
	}
	
	multiplyMatrix(m1, m2);
	
	// Free the memory for m1 and m2
	for (int i = 0; i < R1; i++){
		free(m1[i]);
	}
	free(m1);
	
	for (int j = 0; j < R2; j++){
		free(m2[j]);
	}
	free(m2);
	
	return 0;
}

