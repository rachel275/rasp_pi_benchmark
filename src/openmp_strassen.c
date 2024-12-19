#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void matrix_addition(float* C, const float* A, const float* B, int size) {
    for (int i = 0; i < size * size; i++) {
        C[i] = A[i] + B[i];
    }
}

void matrix_subtraction(float* C, const float* A, const float* B, int size) {
    for (int i = 0; i < size * size; i++) {
        C[i] = A[i] - B[i];
    }
}

void matrix_multiplication(float* C, const float* A, const float* B, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i * size + j] = 0;
            for (int k = 0; k < size; k++) {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
}

void strassen(float* C, const float* A, const float* B, int size) {
    if (size == 2) {
        float M1, M2, M3, M4, M5, M6, M7;
        float A11 = A[0], A12 = A[1], A21 = A[2], A22 = A[3];
        float B11 = B[0], B12 = B[1], B21 = B[2], B22 = B[3];

        M1 = (A11 + A22) * (B11 + B22);
        M2 = (A21 + A22) * B11;
        M3 = A11 * (B12 - B22);
        M4 = A22 * (B21 - B11);
        M5 = (A11 + A12) * B22;
        M6 = (A21 - A11) * (B11 + B12);
        M7 = (A12 - A22) * (B21 + B22);

        C[0] = M1 + M4 - M5 + M7;
        C[1] = M3 + M5;
        C[2] = M2 + M4;
        C[3] = M1 - M2 + M3 + M6;

        return;
    }

    int newSize = size / 2;
    int subMatrixSize = newSize * newSize;

    float *A11 = (float*) malloc(subMatrixSize * sizeof(float));
    float *A12 = (float*) malloc(subMatrixSize * sizeof(float));
    float *A21 = (float*) malloc(subMatrixSize * sizeof(float));
    float *A22 = (float*) malloc(subMatrixSize * sizeof(float));
    float *B11 = (float*) malloc(subMatrixSize * sizeof(float));
    float *B12 = (float*) malloc(subMatrixSize * sizeof(float));
    float *B21 = (float*) malloc(subMatrixSize * sizeof(float));
    float *B22 = (float*) malloc(subMatrixSize * sizeof(float));
    float *C11 = (float*) malloc(subMatrixSize * sizeof(float));
    float *C12 = (float*) malloc(subMatrixSize * sizeof(float));
    float *C21 = (float*) malloc(subMatrixSize * sizeof(float));
    float *C22 = (float*) malloc(subMatrixSize * sizeof(float));

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i * newSize + j] = A[i * size + j];
            A12[i * newSize + j] = A[i * size + j + newSize];
            A21[i * newSize + j] = A[(i + newSize) * size + j];
            A22[i * newSize + j] = A[(i + newSize) * size + j + newSize];

            B11[i * newSize + j] = B[i * size + j];
            B12[i * newSize + j] = B[i * size + j + newSize];
            B21[i * newSize + j] = B[(i + newSize) * size + j];
            B22[i * newSize + j] = B[(i + newSize) * size + j + newSize];
        }
    }

    float *M1 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M2 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M3 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M4 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M5 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M6 = (float*) malloc(subMatrixSize * sizeof(float));
    float *M7 = (float*) malloc(subMatrixSize * sizeof(float));
    float *tempA = (float*) malloc(subMatrixSize * sizeof(float));
    float *tempB = (float*) malloc(subMatrixSize * sizeof(float));

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            matrix_addition(tempA, A11, A22, newSize);
            matrix_addition(tempB, B11, B22, newSize);
            strassen(M1, tempA, tempB, newSize);
        }

        #pragma omp section
        {
            matrix_addition(tempA, A21, A22, newSize);
            strassen(M2, tempA, B11, newSize);
        }

        #pragma omp section
        {
            matrix_subtraction(tempB, B12, B22, newSize);
            strassen(M3, A11, tempB, newSize);
        }

        #pragma omp section
        {
            matrix_subtraction(tempB, B21, B11, newSize);
            strassen(M4, A22, tempB, newSize);
        }

        #pragma omp section
        {
            matrix_addition(tempA, A11, A12, newSize);
            strassen(M5, tempA, B22, newSize);
        }
    }

    matrix_addition(C11, M1, M4, newSize);
    matrix_subtraction(C11, C11, M5, newSize);
    matrix_addition(C11, C11, M7, newSize);

    matrix_addition(C12, M3, M5, newSize);
    matrix_addition(C21, M2, M4, newSize);

    matrix_subtraction(C22, M1, M2, newSize);
    matrix_addition(C22, C22, M3, newSize);
    matrix_addition(C22, C22, M6, newSize);
}
