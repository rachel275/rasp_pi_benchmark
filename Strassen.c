#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAT_SIZE
#define MAT_SIZE 1024
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE

double** add(double** temp, double** a, double** b) {
	for (int i = 0; i < R1/2; i++) {
		for (int j = 0; j < R1/2; j++) {
			temp[i][j] = a[i][j] + b[i][j];
		}
	}
	return temp;
}

double** sub(double** temp, double** a, double** b) {
	for (int i = 0; i < R1/2; i++) {
		for (int j = 0; j < R1/2; j++) {
			temp[i][j] = a[i][j] - b[i][j];
		}
	}
	return temp;
}

double** mult(double** result, double** a, double** b) {
	for (int i = 0; i < R1/2; i++) {
		for (int j = 0; j < C2/2; j++) {
			result[i][j] = 0.0;
			for (int k = 0; k < R2/2; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return result;
}

void Strassen(double** a, double** b) {
	double **a11 = (double**)malloc((R1/2) * sizeof(double *));
	double **a12 = (double**)malloc((R1/2) * sizeof(double *));
	double **a21 = (double**)malloc((R1/2) * sizeof(double *));
	double **a22 = (double**)malloc((R1/2) * sizeof(double *));
	double **b11 = (double**)malloc((R1/2) * sizeof(double *));
	double **b12 = (double**)malloc((R1/2) * sizeof(double *));
	double **b21 = (double**)malloc((R1/2) * sizeof(double *));
	double **b22 = (double**)malloc((R1/2) * sizeof(double *));
	double **m1 = (double**)malloc(R1/2 * sizeof(double *));
	double **m2 = (double**)malloc(R1/2 * sizeof(double *));
	double **m3 = (double**)malloc(R1/2 * sizeof(double *));
	double **m4 = (double**)malloc(R1/2 * sizeof(double *));
	double **m5 = (double**)malloc(R1/2 * sizeof(double *));
	double **m6 = (double**)malloc(R1/2 * sizeof(double *));
	double **m7 = (double**)malloc(R1/2 * sizeof(double *));
	double **temp1 = (double**)malloc(R1/2 * sizeof(double *));
	double **temp2 = (double**)malloc(R1/2 * sizeof(double *));

	for (int i = 0; i < R1/2; i++) {
		a11[i] = (double* )malloc ((C1/2) * sizeof(double));
		a12[i] = (double* )malloc ((C1/2) * sizeof(double));
		a21[i] = (double* )malloc ((C1/2) * sizeof(double));
		a22[i] = (double* )malloc ((C1/2) * sizeof(double));
		b11[i] = (double* )malloc ((C1/2) * sizeof(double));
		b12[i] = (double* )malloc ((C1/2) * sizeof(double));
		b21[i] = (double* )malloc ((C1/2) * sizeof(double));
		b22[i] = (double* )malloc ((C1/2) * sizeof(double));
		m1[i] = (double* )malloc ((C1/2) * sizeof(double));
		m2[i] = (double* )malloc ((C1/2) * sizeof(double));
		m3[i] = (double* )malloc ((C1/2) * sizeof(double));
		m4[i] = (double* )malloc ((C1/2) * sizeof(double));
		m5[i] = (double* )malloc ((C1/2) * sizeof(double));
		m6[i] = (double* )malloc ((C1/2) * sizeof(double));
		m7[i] = (double* )malloc ((C1/2) * sizeof(double));
		temp1[i] = (double* )malloc ((C1/2) * sizeof(double));
		temp2[i] = (double* )malloc ((C1/2) * sizeof(double));
	}

	m1 = mult(m1, add(temp1, a11, a22), add(temp2, b11, b22));
	m2 = mult(m2, add(temp1, a21, a22), b11);
	m3 = mult(m3, a11, sub(temp1, b12, b22));
	m4 = mult(m4, a22, sub(temp1, b21, b11));
	m5 = mult(m5, add(temp1, a11, a12), b22);
	m6 = mult(m6, sub(temp1, a12, a11), add(temp2, b11, b12));
	m7 = mult(m7, sub(temp1, a12, a22), add(temp2, b21, b22));
}

int main() {
	double **a1 = (double**)malloc(R1 * sizeof(double *));
	double **b1 = (double**)malloc(R2 * sizeof(double *));

	for (int i = 0; i < R1; i++) {
		a1[i] = (double* )malloc (C1 * sizeof(double));
	}

	for (int j = 0; j < R2; j++) {
		b1[j] = (double* )malloc (C2 * sizeof(double));
	}

	srand(time(NULL));
	
	for(int o = 0; o < R1; o++) {
		for(int n = 0; n < C1; n++) {
			a1[o][n] = (double)rand() / RAND_MAX;
		}
	}

	for(int p = 0; p < R2; p++) {
		for(int q = 0; q < C2; q++) {
			b1[p][q] = (double)rand() / RAND_MAX;
		}
	}
	
	if (C1 != R2) {
		printf("The number of columns in Matrix-1 must be equal to the number of rows in Matrix-2\n");
		exit(EXIT_FAILURE);
	}
	
	Strassen(a1, b1);
	
	return 0;
}
