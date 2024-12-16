#include <omp.h>

void strassen_2x2(float64x2_t* C, float64x2_t* A, float64x2_t* B) {
    float64x2_t M1[2], M2[2], M3[2], M4[2], M5[2], M6[2], M7[2];
    float64x2_t tempA[2], tempB[2];

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // M1 = (A11 + A22) * (B11 + B22)
            matrix_addition_2x2(tempA, A, &A[2]);
            matrix_addition_2x2(tempB, B, &B[2]);
            matrix_multiplication_2x2(M1, tempA, tempB);
        }

        #pragma omp section
        {
            // M2 = (A21 + A22) * B11
            matrix_addition_2x2(tempA, &A[2], &A[4]);
            matrix_multiplication_2x2(M2, tempA, B);
        }

        #pragma omp section
        {
            // M3 = A11 * (B12 - B22)
            matrix_subtraction_2x2(tempB, &B[1], &B[3]);
            matrix_multiplication_2x2(M3, A, tempB);
        }

        #pragma omp section
        {
            // M4 = A22 * (B21 - B11)
            matrix_subtraction_2x2(tempB, &B[2], B);
            matrix_multiplication_2x2(M4, &A[3], tempB);
        }

        #pragma omp section
        {
            // M5 = (A11 + A12) * B22
            matrix_addition_2x2(tempA, A, &A[1]);
            matrix_multiplication_2x2(M5, tempA, &B[3]);
        }

        #pragma omp section
        {
            // M6 = (A21 - A11) * (B11 + B12)
            matrix_subtraction_2x2(tempA, &A[2], A);
            matrix_addition_2x2(tempB, B, &B[1]);
            matrix_multiplication_2x2(M6, tempA, tempB);
        }

        #pragma omp section
        {
            // M7 = (A12 - A22) * (B21 + B22)
            matrix_subtraction_2x2(tempA, &A[1], &A[3]);
            matrix_addition_2x2(tempB, &B[2], &B[3]);
            matrix_multiplication_2x2(M7, tempA, tempB);
        }
    }

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
