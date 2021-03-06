#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// This is a version of matmul designed to time matrix multiplication using
// different sized matrices.
// The multiplication is performed a number of times and the average time is recorded

// matrix a is l x m
// matrix b is m x n
// matrix c is l x n
void mat_mul(double **a, double **b, double **c, int l, int m, int n){
	int i, j, k;
	for(i = 0; i < l; i++){
		for(j = 0; j < n; j++){
			double sum = 0.0;
			for(k = 0; k < m; k++){
				sum += a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
	}
}

void free_matrix(double **mat){
	free(mat[0]);
	free(mat);
}

double **create_empty_matrix(int m, int n){
	double **result = malloc(sizeof(double) * m);
	double *mat = malloc(sizeof(double) * m * n);
	int i;
	for(i = 0; i < m; i++){
		result[i] = &(mat[i * n]);
	}
	return result;
}

double **create_matrix_with_random_values(int m, int n){
	double **mat = create_empty_matrix(m, n);
	int i, j;
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){
			mat[i][j] = rand() % 5; // numbers in range 0-4
			mat[i][j] -= 2; // adjust range to -2-+2
		}
	}
	return mat;
}

void print_matrix(double **mat, int m, int n){
	int i, j;
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){
			printf("%f\t ", mat[i][j]);
		}
		printf("\n");
	}
}

long long time_matmul(int size){
	double **a = create_matrix_with_random_values(size, size);
	double **b = create_matrix_with_random_values(size, size);
	double **c = create_empty_matrix(size, size);
	struct timeval start_time;
	struct timeval end_time;
	long long time_taken;
	gettimeofday(&start_time, NULL);
	mat_mul(a, b, c, size, size, size);
	gettimeofday(&end_time, NULL);
	time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000L + (end_time.tv_usec - start_time.tv_usec);
	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	return time_taken;
}

#define NUM_ITERATIONS 20
#define NUM_SIZES 24

const int sizes[NUM_SIZES] = { 
	25, 50, 75, 100,
	125, 150, 175, 200,
	225, 250, 275, 300,
	325, 350, 375, 400,
	425, 450, 475, 500, 
	525, 550, 575, 600
};

int main(int argc, char *argv[]){
	srandom(time(NULL));
	int i;

	for(i = 0; i < NUM_SIZES; i++){
		long long time_taken = 0L;
		int j;
		for(j = 0; j < NUM_ITERATIONS; j++){
			time_taken += time_matmul(sizes[i]);
		}
		time_taken = time_taken / NUM_ITERATIONS;
		printf("For size=%d the average time taken across %d runs was: %lld microseconds\n", sizes[i], NUM_ITERATIONS, time_taken);
		//printf("%d\t%lld\n", sizes[i], time_taken);
	}
	return 0;
}
