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

struct arg_struct {
	int** args1;
	int** args2;
};

void multiplyMatrix(int a[][C1], int b[][C2]){
	
	int c[R1][C2];
	
	/** initialise matrix c**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			c[i][j] = 0;
		}
	}
	
	/**The three outermost loops (lines 1-3) share a similar appearance. Their indexes are called ii, jj and kk. All three of them go from zero to n, and all the indexes increase by tile_size.
	* The three innermost loops (lines 4-6) also share a similar appearance. All three have a constant number of iterations which is tile_size. The loop over i starts from ii and ends with ii + tile_size. The loop over j starts from jj and ends with jj + tile_size.
	*  A similar observation applies to the loop over k.
	* Traditionally, loop variables in the inner loops will have names i, j, etc, whereas the loop variables in the outer loops will have names where the letter is duplicated ii, jj, etc. There are also other ways of naming the variables for loop tiling, e.g. outer 
	* loop variables can have names i0, j0 and k0.**/


	/** the iterations in this loop can run independently?? so how do we parallise this to improve the speedup **/
	/** look at parallelising the simple mat mult as well *//
	for (int ii = 0; ii < n; ii += TILESIZE) {
		for (int kk = 0; kk < n; kk += TILESIZE) {
			for (int jj = 0; jj < n; jj += TILESIZE) {
				for (int i = ii; i < ii + TILESIZE; i++) {
					for (int k = kk; k < kk + TILESIZE; k++) {
						for (int j = jj; j < jj + TILESIZE; j++) {
							c[i][j] = c[i][j] + a[i][k] * b[k][j];
						}
					}
				}
			}
		}
	}
}

int main()
{
	//int m1[R1][C1] = {{1, 1}, {2, 2}};
	
//	int m2[R2][C2] = {{1, 1, 1}, {2, 2, 2}};


	int m1[R1][C1];
	int m2[R2][C2];

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

	pthread_t* threads;
	threads = (pthread_t*)malloc(4 * sizeof(pthread_t));
	
	struct arg_struct args;
	args.arg1 = m1;
	args.arg2 = m2;

	int k = 0;
	for (int y = 0; y < 4; y++){
		pthread_create(&threads[y], NULL, &multiplyMatrix, (void*)&args);
	}

	for (int z = 0; z < 4; z++){
		pthread_join(threads[z], (void**)&k);
	}
	return 0;
}
