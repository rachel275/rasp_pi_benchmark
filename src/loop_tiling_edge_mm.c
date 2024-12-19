#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef TILESIZE
#define TILESIZE 5
#endif

#ifndef MAT_SIZE
#define MAT_SIZE 500
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE
#define n MAT_SIZE

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void multiplyMatrix(double m1[][C1], double m2[][C2]){
	
	double result[R1][C2];
	
	/** initialise matrix c**/
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			result[i][j] = 0.0;
		}
	}
	
	/**have to use architecture instrinsics to truly optimise for edge cases when the matrix is not square and the tiles are not of equal size**/
	for (int ii = 0; ii < n; ii += TILESIZE) {
		for (int kk = 0; kk < n; kk += TILESIZE) {
			for (int jj = 0; jj < n; jj += TILESIZE) {
				for (int i = ii; i < MIN(ii + TILESIZE, n); i++) {
					for (int k = kk; k < MIN(kk + TILESIZE, n); k++) {
						int j_end = jj + TILESIZE;
						if (j_end <= n) {
							for (int j = jj; j < j_end; j++) {
								result[i][j] += m1[i][k] * m2[k][j];
							}
						} else {
							for (int j = jj; j < n; j++) {
								result[i][j] += m1[i][k] * m2[k][j];
							}
						}
					}
				}
			}
		}
	}
}

int main()
{
	double m1[R1][C1];
	double m2[R2][C2];

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
