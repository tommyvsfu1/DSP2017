#include "hmm.h"
#include <math.h>

#define T 500000
#define N 5

double A[T][N], B[T][N], G[T][N], E[T-1][N][N];

int main( int argc, char* argv[] ) {
	// ./train iteration model_init.txt seq_model_01.txt model_01.txt
	int iteration = atoi(argv[1]);

	HMM model;
	loadHMM( &model, argv[2] );

	#define a model.transition
	#define b model.observation

	char buff[T];
	FILE *fp = open_or_die( argv[3], "r");
	fread(buff, sizeof(char), T, fp);
	fclose(fp);

	int data[T];
	for( int i=0; i<T; i++ )
		data[i] = buff[i] - 'A';

	while( iteration-- ) {

		// calculate alpha
		// double A[T][N];
		for( int i=0; i<N; i++ )
			A[0][i] = model.initial[i] * b[data[0]][i];

		for( int t=1; t<T; t++ ) {
			for( int j=0; j<N; j++ ) {
				A[t][j] = 1;
				for( int i=0; i<N; i++ )
					A[t][j] += A[t-1][i] * a[i][j];
				A[t][j] *= b[data[t]][j];
			}
		}

		double prob=0;
		for( int i=0; i<N; i++ )
			prob += A[T-1][i];

		// calculate beta
		// double B[T][N];
		for( int i=0; i<N; i++ )
			B[T-1][i] = 1;
		for( int t=T-2; t>=0; t-- ) {
			for( int i=0; i<N; i++ ) {
				B[t][i] = 0;
				for( int j=0; j<N; j++ )
					B[t][i] += a[i][j] * b[data[t+1]][j] * B[t+1][j];
			}
		}

		// calculate gamma
		// double G[T][N];
		for( int t=0; t<T; t++ )
			for( int i=0; i<N; i++ )
				G[t][i] = ( A[t][i] * B[t][i] ) / prob;

		// calculate epsilon
		// double E[T-1][N][N];
		for( int t=0; t<T-1; t++ ) {
			double numerator = 0;

			for( int i=0; i<N; i++ )
				for( int j=0; j<N; j++) {
					E[t][i][j] = A[t][i] * a[i][j] * b[data[t+1]][j] * B[t+1][j];
					numerator += E[t][i][j];
				}

			for( int i=0; i<N; i++ )
				for( int j=0; j<N; j++ )
					E[t][i][j] /= numerator;
		}

		// recalculate parameters

	}

	dumpHMM( stderr, &model );

	return 0;
}