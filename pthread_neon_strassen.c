#include <arm_neon.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

typedef struct {
    float64x2_t* C;
    float64x2_t* A;
    float64x2_t* B;
} ThreadData;

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

void* compute_strassen_2x2(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    matrix_multiplication_2x2(data->C, data->A, data->B);
    return NULL;
}

void strassen_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    float64x2_t M1[2], M2[2], M3[2], M4[2], M5[2], M6[2], M7[2];
    float64x2_t tempA[2], tempB[2];
    pthread_t threads[7];
    ThreadData threadData[7];
    
    matrix_addition_2x2(tempA, A, &A[2]);
    matrix_addition_2x2(tempB, B, &B[2]);
    threadData[0] = (ThreadData){M1, tempA, tempB};
    pthread_create(&threads[0], NULL, compute_strassen_2x2, &threadData[0]);
    
    matrix_addition_2x2(tempA, &A[2], &A[4]);
    threadData[1] = (ThreadData){M2, tempA, B};
    pthread_create(&threads[1], NULL, compute_strassen_2x2, &threadData[1]);
    
    matrix_subtraction_2x2(tempB, &B[1], &B[3]);
    threadData[2] = (ThreadData){M3, A, tempB};
    pthread_create(&threads[2], NULL, compute_strassen_2x2, &threadData[2]);
    
    matrix_subtraction_2x2(tempB, &B[2], B);
    threadData[3] = (ThreadData){M4, &A[3], tempB};
    pthread_create(&threads[3], NULL, compute_strassen_2x2, &threadData[3]);
    
    matrix_addition_2x2(tempA, A, &A[1]);
    threadData[4] = (ThreadData){M5, tempA, &B[3]};
    pthread_create(&threads[4], NULL, compute_strassen_2x2, &threadData[4]);
    
    matrix_subtraction_2x2(tempA, &A[2], A);
    matrix_addition_2x2(tempB, B, &B[1]);
    threadData[5] = (ThreadData){M6, tempA, tempB};
    pthread_create(&threads[5], NULL, compute_strassen_2x2, &threadData[5]);
    
    matrix_subtraction_2x2(tempA, &A[1], &A[3]);
    matrix_addition_2x2(tempB, &B[2], &B[3]);
    threadData[6] = (ThreadData){M7, tempA, tempB};
    pthread_create(&threads[6], NULL, compute_strassen_2x2, &threadData[6]);
    
    for (int i = 0; i < 7; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    matrix_addition_2x2(C, M1, M4);
    matrix_subtraction_2x2(C, C, M5);
    matrix_addition_2x2(C, C, M7);
    
    matrix_addition_2x2(&C[1], M3, M5);
    matrix_addition_2x2(&C[2], M2, M4);
    matrix_subtraction_2x2(&C[3], M1, M2);
    matrix_addition_2x2(&C[3], &C[3], M3);
    matrix_addition_2x2(&C[3], &C[3], M6);
}

int main() {
    float64x2_t A[8], B[8], C[8];
    for (int i = 0; i < 8; i++) {
        A[i] = vdupq_n_f64(i + 1);
        B[i] = vdupq_n_f64(i + 2);
    }
    
    strassen_2x2(C, A, B);
    
    for (int i = 0; i < 8; i++) {
        double result[2];
        vst1q_f64(result, C[i]);
        printf("C[%d]: %lf %lf\n", i, result[0], result[1]);
    }
    return 0;
}
