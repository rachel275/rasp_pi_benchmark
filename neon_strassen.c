#include <arm_neon.h>
#include <stdlib.h>
#include <stdio.h>

void matrix_addition(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    for (int i = 0; i < n * n; ++i) {
        C[i] = vaddq_f64(A[i], B[i]);
    }
}

void matrix_subtraction(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    for (int i = 0; i < n * n; ++i) {
        C[i] = vsubq_f64(A[i], B[i]);
    }
}

void matrix_multiplication(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i * n + j] = vdupq_n_f64(0.0);
            for (int k = 0; k < n; ++k) {
                C[i * n + j] = vfmaq_f64(C[i * n + j], A[i * n + k], B[k * n + j]);
            }
        }
    }
}

void strassen(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    if (n == 2) {
        matrix_multiplication(C, A, B, 2);
    } else {
        int new_size = n / 2;
        int block_size = new_size * new_size;
        
        float64x2_t *A11 = A;
        float64x2_t *A12 = A + block_size;
        float64x2_t *A21 = A + 2 * block_size;
        float64x2_t *A22 = A + 3 * block_size;

        float64x2_t *B11 = B;
        float64x2_t *B12 = B + block_size;
        float64x2_t *B21 = B + 2 * block_size;
        float64x2_t *B22 = B + 3 * block_size;

        float64x2_t *C11 = C;
        float64x2_t *C12 = C + block_size;
        float64x2_t *C21 = C + 2 * block_size;
        float64x2_t *C22 = C + 3 * block_size;
        
        float64x2_t *M1 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M2 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M3 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M4 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M5 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M6 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *M7 = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);

        float64x2_t *tempA = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        float64x2_t *tempB = (float64x2_t*)malloc(sizeof(float64x2_t) * block_size);
        
        matrix_addition(tempA, A11, A22, new_size);
        matrix_addition(tempB, B11, B22, new_size);
        strassen(M1, tempA, tempB, new_size);

        matrix_addition(tempA, A21, A22, new_size);
        strassen(M2, tempA, B11, new_size);

        matrix_subtraction(tempB, B12, B22, new_size);
        strassen(M3, A11, tempB, new_size);

        matrix_subtraction(tempB, B21, B11, new_size);
        strassen(M4, A22, tempB, new_size);

        matrix_addition(tempA, A11, A12, new_size);
        strassen(M5, tempA, B22, new_size);

        matrix_subtraction(tempA, A21, A11, new_size);
        matrix_addition(tempB, B11, B12, new_size);
        strassen(M6, tempA, tempB, new_size);

        matrix_subtraction(tempA, A12, A22, new_size);
        matrix_addition(tempB, B21, B22, new_size);
        strassen(M7, tempA, tempB, new_size);

        matrix_addition(C11, M1, M4, new_size);
        matrix_subtraction(C11, C11, M5, new_size);
        matrix_addition(C11, C11, M7, new_size);

        matrix_addition(C12, M3, M5, new_size);

        matrix_addition(C21, M2, M4, new_size);

        matrix_subtraction(C22, M1, M2, new_size);
        matrix_addition(C22, C22, M3, new_size);
        matrix_addition(C22, C22, M6, new_size);

        free(M1); free(M2); free(M3); free(M4);
        free(M5); free(M6); free(M7);
        free(tempA); free(tempB);
    }
}

int main() {
    int n = 4; // Size of the matrix (must be power of 2)
    int size = n * n;
    float64x2_t *A = (float64x2_t*)malloc(sizeof(float64x2_t) * size);
    float64x2_t *B = (float64x2_t*)malloc(sizeof(float64x2_t) * size);
    float64x2_t *C = (float64x2_t*)malloc(sizeof(float64x2_t) * size);

    for (int i = 0; i < size; i++) {
        A[i] = vdupq_n_f64(i + 1);
        B[i] = vdupq_n_f64(i + 2);
    }

    strassen(C, A, B, n);

    for (int i = 0; i < size; i++) {
        double result[2];
        vst1q_f64(result, C[i]);
        printf("C[%d]: %lf %lf\n", i, result[0], result[1]);
    }

    free(A);
    free(B);
    free(C);
    return 0;
}
