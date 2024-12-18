/* 
    assumptions: 
        * matrices are just (one-dimensional) arrays of double-precision floating point numbers
        * all matrices are square
        * the length of any matrix is a multiple of 4 
        * entries in a matrix are stored in column-major order
    
    this header provides four different levels of optimization for GEMM:
        1. multithreaded (OpenMP) 
        2. parallel (MPI) 
        3. vectorized (NEON)
        4. multithreaded, parallel, and vectorized (OpenMP + MPI + NEON)

    note: MPI calls are made inside main functions
*/

/** https://github.com/alvarezpj/gemm-raspberrypi-cluster/blob/master/src/neon.c **/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <arm_neon.h>

#ifndef TILESIZE
#define TILESIZE 128
#endif

#ifndef MAT_SIZE
#define MAT_SIZE 1024
#endif

#define R1 MAT_SIZE
#define C1 MAT_SIZE
#define R2 MAT_SIZE
#define C2 MAT_SIZE
#define LENGTH MAT_SIZE

// number of elements per vector register
#define VECREG_LEN                             2 // Double precision vectors hold 2 elements in NEON
// vector load
#define VECLOD(ptr)                            vld1q_f64(ptr) 
// vector multiply by scalar
#define VMULSC(vecreg, scalar)                 vmulq_n_f64(vecreg, scalar)
// vector multiply accumulate
#define VMULAC(vecreg1, vecreg2, vecreg3)      vmlaq_f64(vecreg1, vecreg2, vecreg3)
// extract lanes from a vector
#define VEXTLN(vecreg, lane)                   vgetq_lane_f64(vecreg, lane)


/***********************************************************************************
*  Miscellaneous                                                                   *
***********************************************************************************/

/* initialize matrix */
void mxinitd(size_t len, double *mx, size_t mod)
{
    size_t i;

    for(i = 0; i < (len * len); i++)
        *(mx + i) = (double)((i * i) % mod);
}

/* initialize matrix with random double-precision floating point numbers in range [0.0, upper bound] */
void rmxinitd(size_t len, double *mx, double ubound)
{
    size_t i;
    srand(time(0));

    for(i = 0; i < (len * len); i++)
        *(mx + i) = ((double)rand() / (double)RAND_MAX) * ubound;
}

/* matrix transpose */
void mxtransposed(size_t len, double *mx)
{
    size_t i, j;
    double tmp;

    for(i = 0; i < len; i++)
    {
        for(j = (i + 1); j < len; j++)
        {
            tmp = *(mx + (len * j) + i);
            *(mx + (len * j) + i) = *(mx + (len * i) + j);
            *(mx + (len * i) + j) = tmp;
        }
    }
}

/** NOTE: the following two functions are used to partition matrices **/

/* get start index */
size_t gsid(size_t len, size_t task_id, size_t num_tasks)
{
    return ((task_id * len) / num_tasks);
}

/* get end index */
size_t geid(size_t len, size_t task_id, size_t num_tasks)
{
    return (((task_id + 1) * len) / num_tasks);
}


/***********************************************************************************
*  NEON (SIMD)                                                                     *
***********************************************************************************/

/* vectorized general matrix multiply */
void vmxmultiplyd(size_t len, double *mxa, double *mxb, double *mxc)
{
    float64x2_t a, b, c;
    size_t i, j, k, reps = len / VECREG_LEN;
    double *mxap, *mxbp, *mxcp, tmp;

    // transpose matrix mxa
    for(i = 0; i < len; i++)
    {
        for(j = (i + 1); j < len; j++)
        {
            tmp = *(mxa + (len * j) + i);
            *(mxa + (len * j) + i) = *(mxa + (len * i) + j);
            *(mxa + (len * i) + j) = tmp;
        }
    }

    // multiply
    for(i = 0; i < len; i++)
    {
        mxap = mxa;

        for(j = 0; j < len; j++)
        {
            c = VMULSC(c, 0.0); 
            mxbp = mxb + (len * i);

            for(k = 0; k < reps; k++)
            {
                a = VECLOD(mxap); 
                b = VECLOD(mxbp);
                c = VMULAC(c, a, b); 
                mxap += VECREG_LEN;
                mxbp += VECREG_LEN;
            }

            mxcp = mxc + (len * i) + j;
            // compute entry
            tmp = VEXTLN(c, 0) + VEXTLN(c, 1);
            // store entry in result matrix
            *(mxcp) = tmp; 
        } 
    }
}

int main()
{  
    __builtin___clear_cache;
    double *a = malloc(LENGTH * LENGTH * sizeof(double)); 
    double *b = malloc(LENGTH * LENGTH * sizeof(double));
    double *c = calloc(LENGTH * LENGTH, sizeof(double));

    rmxinitd(LENGTH, a, 8.0);
    rmxinitd(LENGTH, b, 5.0);

    // start timer
    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    vmxmultiplyd(LENGTH, a, b, c);

    // end timer
    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    // cpu_time = (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    // cpu_time += (end.tv_sec - start.tv_sec); 
    // printf("execution time: %.5f s\n", cpu_time);

    free(a);
    free(b);
    free(c);

    return 0;
}
