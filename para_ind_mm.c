

// C Program to multiply two matrix using pthreads without
// use of global variables
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifndef MAT_SIZE
#define MAT_SIZE 128
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE
// Each thread computes single element in the resultant
// matrix

struct arg_struct {
   int** arg1;
   int** arg2;
};

/**now to initialise this in a c file**/ 
void* multiplyMatrix(void *arguments){
	
	struct arg_struct *args = (struct arg_struct *)arguments;
	int** m1 = args->arg1;
	int** m2 = args->arg2;

	
	int **result = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		result[i] = (int* )malloc (C1 * sizeof(int));
	}
	
	/** simple loop for matrix multiplication**/
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++){
			result[i][j] = 0;
			for (int k = 0; k < R2; k++){
				//int temp = args->arg1[i][k] + 1;
				result[i][j] = result[i][j] + args->arg1[i][k] * args->arg2[k][j];
			}
			
		}
	}
    // Used to terminate a thread and the return value is
    // passed as a pointer
    pthread_exit(NULL);
}

// Driver code
int main()
{

    	__builtin___clear_cache;	
	int **matA = (int**)malloc(R1 * sizeof(int *));
	for (int i = 0; i < R1; i++){
		matA[i] = (int* )malloc (C1 * sizeof(int));
	}

	int **matB = (int**)malloc(R2 * sizeof(int *));
	for (int j = 0; j < R2; j++){
		matB[j] = (int* )malloc (C2 * sizeof(int));
	}

	srand(time(NULL));	
	
	for(int o = 0; o < R1; o++){
		for(int n = 0; n < C1; n++){
			matA[o][n] = rand();
	//		printf(" %i", m1[o][n]);
		} 
	}

	for(int p = 0; p < R2; p++){
		for(int q = 0; q < C2; q++){
			matB[p][q] = rand();
		}
	}
   

    	// declaring array of 4 threads, 4 cpus
    	pthread_t* threads;
    	threads = (pthread_t*)malloc(4 * sizeof(pthread_t));

    	//now we want to send both matrices to each thread
    	struct arg_struct args;
    	args.arg1 = matA;
    	args.arg2 = matB;

    	int k = 0;
    	for (int y = 0; y < 4; y++){
    		pthread_create(&threads[y], NULL, &multiplyMatrix, (void*)&args);
    	}

    	//printf("RESULTANT MATRIX IS :- \n");
    	for (int z = 0; z < 4; z++) {
        	// Joining all threads and collecting return value
        	pthread_join(threads[z], (void**)&k);
    	}
    	return 0;
}
