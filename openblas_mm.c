#include <stdlib.h>
#include <cblas.h>

void matrix_multiply(double *A, double *B, double *C, int M, int N, int K){
	//set alpha and beta values for multiplication
	double alpha = 1.0;
	double beta = 0.0;

	//perform C = alpha * A * B + beta * C
	//A: MxK matrix
	//b: KxN matrix
	//C: MxN matrix
	cblas_dgemm(CblasRowMajor, CblasNoTran, CblasNoTrans, //Row major order, with no transpositions 
			M, N, K, alpha, //defines the sizes of the matrices 
			A, K, 
			B, N, 
			beta, 
			C, N);


}

int main(){
	int M, N, K;



	matrix_multiply(A, B, C, M, N, K);

	return 0;
}
