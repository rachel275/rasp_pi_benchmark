/** https://github.com/darshan14/matrix-multiplication-openMP/blob/master/MatrixMultiplication.c **/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <sys/time.h>


#ifndef TILESIZE
#define TILESIZE 128
#endif

#ifndef MAT_SIZE
#define MAT_SIZE 1024
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define N MAT_SIZE


#define MAX_DIM N * N

#define min(x,y) (((x)<(y))?(x):(y))

double A[N][N];
double B[N][N];
double C_naive[N][N];
double C_block[N][N];
double vectorA[MAX_DIM];
double vectorB[MAX_DIM];


/*void display(int x[N][N]) {
	int i,j;
	for (i= 0; i< N; i++)
		{
			for (j= 0; j< N; j++)
			{
				printf("%d\t",x[i][j]);
			}
			printf("\n");
		}
}*/

void convert(int dim){
	#pragma omp parallel for
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++){
			vectorA[i * dim + j] = A[i][j];
			vectorB[j * dim + i] = B[i][j];
		}
	}
}

void naiveMultiplication(int dim) {
	int i,j,k;
	#pragma omp parallel for
    	for (i = 0; i < dim; i++) {
        	for (j = 0; j < dim; j++) {
			C_naive[i][j] = 0.0;
            	for (k = 0; k < dim; k++) {
                	C_naive[i][j] += A[i][k] * B[k][j];
            	}
        	}
	}
}


void blockMultiplication(int dim) {
	int i,j,k,bs,ii,jj,kk;
	double sum;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed; 
	bs = TILESIZE;
	
	convert(dim);
	
	#pragma omp parallel for private(i, j, k, ii, jj, kk, sum) shared(C_block) schedule(static)
	for(ii = 0; ii < dim; ii += bs)
		for(jj = 0; jj < dim; jj += bs)
			for(kk = 0; kk < dim; kk += bs)
				for(i = ii; i < min(dim, ii+bs); i++)
					for(j = jj; j < min(dim, jj+bs); j++) {
						sum = 0.0;
						for(k = kk; k < min(dim, kk+bs); k++)
							sum += vectorA[i * dim + k] * vectorB[j * dim + k];
						C_block[i][j] = sum;
					}
}


int main(int argc,char*argv[]) 
{
    int i,j;
    __builtin___clear_cache;
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
		{
           A[i][j] = ((double)rand() / RAND_MAX) * 100.0;
		   B[i][j] = ((double)rand() / RAND_MAX) * 100.0;
		}
		
	omp_set_num_threads(4);
	
	naiveMultiplication(N);
	
	return 0;
}
