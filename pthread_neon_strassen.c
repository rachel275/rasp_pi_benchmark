#include <arm_neon.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define MIN_MATRIX_SIZE 2  // The smallest submatrix to compute directly

typedef struct {
    float64x2_t* C;
    float64x2_t* A;
    float64x2_t* B;
    int size;
} ThreadData;

void matrix_addition(float64x2_t* C, float64x2_t* A, float64x2_t* B, int size) {
    for (int i = 0; i < size * size / 2; ++i) {
        C[i] = vaddq_f64(A[i], B[i]);
    }
}

void matrix_subtraction(float64x2_t* C, float64x2_t* A, float64x2_t* B, int size) {
    for (int i = 0; i < size * size / 2; ++i) {
        C[i] = vsubq_f64(A[i], B[i]);
    }
}

void matrix_multiplication(float64x2_t* C, float64x2_t* A, float64x2_t* B, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size / 2; ++j) {
            C[i * size / 2 + j] = vmulq_f64(A[i * size / 2], B[j]);
            for (int k = 1; k < size; ++k) {
                C[i * size / 2 + j] = vfmaq_f64(C[i * size / 2 + j], A[i * size / 2 + k], B[k * size / 2 + j]);
            }
        }
    }
}

void* compute_strassen(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    matrix_multiplication(data->C, data->A, data->B, data->size);
    return NULL;
}

void strassen(float64x2_t* C, float64x2_t* A, float64x2_t* B, int size) {
    if (size <= MIN_MATRIX_SIZE) {
        matrix_multiplication(C, A, B, size);
        return;
    }

    int newSize = size / 2;
    int subMatrixSize = newSize * newSize / 2;
    float64x2_t *M1 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M2 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M3 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M4 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M5 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M6 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *M7 = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *tempA = malloc(subMatrixSize * sizeof(float64x2_t));
    float64x2_t *tempB = malloc(subMatrixSize * sizeof(float64x2_t));
    
    pthread_t threads[7];
    ThreadData threadData[7];
    
    matrix_addition(tempA, A, A + subMatrixSize * 2, newSize);
    matrix_addition(tempB, B, B + subMatrixSize * 2, newSize);
    threadData[0] = (ThreadData){M1, tempA, tempB, newSize};
    pthread_create(&threads[0], NULL, compute_strassen, &threadData[0]);
    
    matrix_addition(tempA, A + subMatrixSize * 2, A + subMatrixSize * 4, newSize);
    threadData[1] = (ThreadData){M2, tempA, B, newSize};
    pthread_create(&threads[1], NULL, compute_strassen, &threadData[1]);
    
    matrix_subtraction(tempB, B + subMatrixSize, B + subMatrixSize * 3, newSize);
    threadData[2] = (ThreadData){M3, A, tempB, newSize};
    pthread_create(&threads[2], NULL, compute_strassen, &threadData[2]);
    
    matrix_subtraction(tempB, B + subMatrixSize * 2, B, newSize);
    threadData[3] = (ThreadData){M4, A + subMatrixSize * 3, tempB, newSize};
    pthread_create(&threads[3], NULL, compute_strassen, &threadData[3]);
    
    matrix_addition(tempA, A, A + subMatrixSize, newSize);
    threadData[4] = (ThreadData){M5, tempA, B + subMatrixSize * 3, newSize};
    pthread_create(&threads[4], NULL, compute_strassen, &threadData[4]);
    
    matrix_subtraction(tempA, A + subMatrixSize * 2, A, newSize);
    matrix_addition(tempB, B, B + subMatrixSize, newSize);
    threadData[5] = (ThreadData){M6, tempA, tempB, newSize};
    pthread_create(&threads[5], NULL, compute_strassen, &threadData[5]);
    
    matrix_subtraction(tempA, A + subMatrixSize, A + subMatrixSize * 3, newSize);
    matrix_addition(tempB, B + subMatrixSize * 2, B + subMatrixSize * 3, newSize);
    threadData[6] = (ThreadData){M7, tempA, tempB, newSize};
    pthread_create(&threads[6], NULL, compute_strassen, &threadData[6]);
    
    for (int i = 0; i < 7; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    matrix_addition(C, M1, M4, newSize);
    matrix_subtraction(C, C, M5, newSize);
    matrix_addition(C, C, M7, newSize);
    
    matrix_addition(C + subMatrixSize, M3, M5, newSize);
    matrix_addition(C + subMatrixSize * 2, M2, M4, newSize);
    
    matrix_subtraction(C + subMatrixSize * 3, M1, M2, newSize);
    matrix_addition(C + subMatrixSize * 3, C + subMatrixSize * 3, M3, newSize);
    matrix_addition(C + subMatrixSize * 3, C + subMatrixSize * 3, M6, newSize);

    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7);
    free(tempA); free(tempB);
}
