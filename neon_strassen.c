#include <arm_neon.h>
#include <stdlib.h>

void matrix_addition_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    for (int i = 0; i < 2; ++i) {
        C[i] = vaddq_f64(A[i], B[i]);
    }
}

void matrix_subtraction_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    for (int i = 0; i < 2; ++i) {
        C[i] = vsubq_f64(A[i], B[i]);
    }
}

void matrix_multiplication_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    for (int i = 0; i < 2; ++i) {
        float64x2_t a0 = vdupq_n_f64(vgetq_lane_f64(A[i], 0));
        float64x2_t a1 = vdupq_n_f64(vgetq_lane_f64(A[i], 1));
        C[i] = vmulq_f64(a0, B[0]);
        C[i] = vfmaq_f64(C[i], a1, B[1]);
    }
}

void strassen_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    float64x2_t M1[2], M2[2], M3[2], M4[2], M5[2], M6[2], M7[2];
    float64x2_t tempA[2], tempB[2];
    
    // M1 = (A11 + A22) * (B11 + B22)
    matrix_addition_2x2(tempA, A, &A[2]);
    matrix_addition_2x2(tempB, B, &B[2]);
    matrix_multiplication_2x2(M1, tempA, tempB);
    
    // M2 = (A21 + A22) * B11
    matrix_addition_2x2(tempA, &A[2], &A[4]);
    matrix_multiplication_2x2(M2, tempA, B);
    
    // M3 = A11 * (B12 - B22)
    matrix_subtraction_2x2(tempB, &B[1], &B[3]);
    matrix_multiplication_2x2(M3, A, tempB);
    
    // M4 = A22 * (B21 - B11)
    matrix_subtraction_2x2(tempB, &B[2], B);
    matrix_multiplication_2x2(M4, &A[3], tempB);
    
    // M5 = (A11 + A12) * B22
    matrix_addition_2x2(tempA, A, &A[1]);
    matrix_multiplication_2x2(M5, tempA, &B[3]);
    
    // M6 = (A21 - A11) * (B11 + B12)
    matrix_subtraction_2x2(tempA, &A[2], A);
    matrix_addition_2x2(tempB, B, &B[1]);
    matrix_multiplication_2x2(M6, tempA, tempB);
    
    // M7 = (A12 - A22) * (B21 + B22)
    matrix_subtraction_2x2(tempA, &A[1], &A[3]);
    matrix_addition_2x2(tempB, &B[2], &B[3]);
    matrix_multiplication_2x2(M7, tempA, tempB);
    
    // C11 = M1 + M4 - M5 + M7
    matrix_addition_2x2(C, M1, M4);
    matrix_subtraction_2x2(C, C, M5);
    matrix_addition_2x2(C, C, M7);
    
    // C12 = M3 + M5
    matrix_addition_2x2(&C[1], M3, M5);
    
    // C21 = M2 + M4
    matrix_addition_2x2(&C[2], M2, M4);
    
    // C22 = M1 - M2 + M3 + M6
    matrix_subtraction_2x2(&C[3], M1, M2);
    matrix_addition_2x2(&C[3], &C[3], M3);
    matrix_addition_2x2(&C[3], &C[3], M6);
}

void matrix_multiply_strassen(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    if (n == 2) {
        strassen_2x2(C, A, B);
    } else {
        int new_size = n / 2;
        float64x2_t* A11 = A;
        float64x2_t* A12 = A + new_size;
        float64x2_t* A21 = A + new_size * n;
        float64x2_t* A22 = A + new_size * n + new_size;

        float64x2_t* B11 = B;
        float64x2_t* B12 = B + new_size;
        float64x2_t* B21 = B + new_size * n;
        float64x2_t* B22 = B + new_size * n + new_size;

        float64x2_t* C11 = C;
        float64x2_t* C12 = C + new_size;
        float64x2_t* C21 = C + new_size * n;
        float64x2_t* C22 = C + new_size * n + new_size;

        float64x2_t *M1 = (float64x2_t*)malloc(sizeof(float64x2_t) * new_size * new_size);
        float64x2_t *M2 = (float64x2_t*)malloc(sizeof(float64x2_t) * new_size * new_size);

        matrix_multiply_strassen(C11, A11, B11, new_size);
        matrix_multiply_strassen(C12, A12, B21, new_size);
        matrix_multiply_strassen(C21, A21, B11, new_size);
        matrix_multiply_strassen(C22, A22, B21, new_size);

        free(M1);
        free(M2);
    }
}

int main() {
    float64x2_t A[8], B[8], C[8];
    for (int i = 0; i < 8; i++) {
        A[i] = vdupq_n_f64(i + 1);
        B[i] = vdupq_n_f64(i + 2);
    }
    
    matrix_multiply_strassen(C, A, B, 2);
    
    for (int i = 0; i < 8; i++) {
        double result[2];
        vst1q_f64(result, C[i]);
        printf("C[%d]: %lf %lf\n", i, result[0], result[1]);
    }
    return 0;
}
