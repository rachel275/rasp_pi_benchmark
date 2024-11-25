
// C Program to multiply two matrix using pthreads without
// use of global variables
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifndef MAT_SIZE
#define MAT_SIZE 4
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE
// Each thread computes single element in the resultant
// matrix

void* mult(void* arg)
{
    int* data = (int*)arg;
    int k = 0, i = 0;

    int x = data[0];
    for (i = 1; i <= x; i++)
        k += data[i] * data[i + x];

    int* p = (int*)malloc(sizeof(int));
    *p = k;

    // Used to terminate a thread and the return value is
    // passed as a pointer
    pthread_exit(p);
}

// Driver code
int main()
{

    __builtin___clear_cache;
    int matA[MAT_SIZE][MAT_SIZE];
    int matB[MAT_SIZE][MAT_SIZE];

    int i, j, k;

    // Generating random values in matA
    for (i = 0; i < R1; i++)
        for (j = 0; j < C1; j++)
            matA[i][j] = rand() % 10;

    // Generating random values in matB
    for (i = 0; i < R1; i++)
        for (j = 0; j < C1; j++)
            matB[i][j] = rand() % 10;

    int max = R1 * C2;

    // declaring array of threads of size r1*c2
    pthread_t* threads;
    threads = (pthread_t*)malloc(max * sizeof(pthread_t));

    int count = 0;
    int* data = NULL;
    for (i = 0; i < R1; i++)
        for (j = 0; j < C2; j++) {

            // storing row and column elements in data
            data = (int*)malloc((20) * sizeof(int));
            data[0] = C1;

            for (k = 0; k < C1; k++)
                data[k + 1] = matA[i][k];

            for (k = 0; k < R2; k++)
                data[k + C1 + 1] = matB[k][j];

            // creating threads
            pthread_create(&threads[count++], NULL, mult,
                           (void*)(data));
        }

    //printf("RESULTANT MATRIX IS :- \n");
    for (i = 0; i < max; i++) {
        // Joining all threads and collecting return value
        pthread_join(threads[i], (void**)&k);
    }

    return 0;
}
