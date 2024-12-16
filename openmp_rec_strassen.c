#include <omp.h>

void matrix_multiply_strassen(float64x2_t* C, float64x2_t* A, float64x2_t* B, int n) {
    if (n == 2) {
        strassen_2x2(C, A, B);
    } else {
        int new_size = n / 2;

        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task
                matrix_multiply_strassen(C, A, B, new_size);

                #pragma omp task
                matrix_multiply_strassen(C + new_size, A + new_size, B + new_size * n, new_size);

                #pragma omp task
                matrix_multiply_strassen(C + new_size * n, A + new_size * n, B, new_size);

                #pragma omp task
                matrix_multiply_strassen(C + new_size * n + new_size, A + new_size * n + new_size, B + new_size * n + new_size, new_size);
            }
        }
    }
}
