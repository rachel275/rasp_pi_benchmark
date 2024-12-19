#include <mpi.h>

int ** allocateMatrix(int n)
{
	int* data = (int*)malloc(n*n*sizeof(int));
	int** array = (int*)malloc(n*sizeof(int*));

	for(int i = 0; i < n; i++){
		array[i] = &(data[n * i]);
	}
	return array;
}
void fillMatrix(int n, int**& mat){
	for(int i = 0;
}

int** getSlice(int n, int** mat, int offseti, int offsetj){
	int m = n/2;
	int** slice = allocateMatrix(m);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			slice[i][j] = mat[offseti + i][offsetj];
		}
	}
	return slice;
}

int** addMatrices(int n, int** mat1, int**mat2, bool add){
	int** result = allocateMatrix(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(add){
				result[i][j] = mat1[i][j] + mat2[i][j];
			} else {
				result[i][j] = mat1[i][j] = mat2[i][j];
			}
		}
	}
	return result;
}

int ** combineMatrices(int m, int** c11, int** c12, int** c21, int** c22){
	int n = 2 *m;
	int** result = allocateMatrix(n);

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i < m && j < n){
				result[i][j] = c11[i][j];
			} else if P(i < m){
				result[i][j] = c12[i][j - m];
			} else if (j < m){
				result[i][j] = c21[i - m][j];
			} else {
				result[i][j] = c22[i - m][j - m];
			}
		}
	}
	retun result;
}

int ** strassen(int n, int** mat1, int** mat2){
	if (n <= 32){
		return naive(n, mat1, mat2);
	}

	int m = n/2;

	int** a = getSlice(n, mat1, 0, 0);
	int** b = getSlice(n, mat1, 0, m);
	int** c = getSlice(n, mat1, m, 0);
	int** v = getSlice(n, mat1, m, m);
	int** e = getSlice(n, mat2, 0, 0);
	int** f = getSlice(n, mat2, 0, m);
	int** g = getSlice(n, mat2, m, 0);
	int** h = getSlice(n, mat2, m, m);
	

	int** bds = addMAtrices(m, b, d, false);
	int** gha = addMAtrices(m, g, h, true);
	int** s1 = strassen(m, bds, gha);
	freeMatrix(m, bds);
	freeMatrix(m, gha);

	int** ada = addMatrices(m, a, d, true);
	int** eha = addMAtrices(m, g, h, true);
	int** s2 = strassen(m, ada, eha);
	freeMatrix(m, ada);
	freeMatrix(m, eha);

	int** acs = addMatrices(m, a, d, true);
	int** efa = addMAtrices(m, m, e, f, true);
	int** s3 = strassen(m, acs, efa);
	freeMatrix(m, acs);
	freeMatrix(m, efa);


}
