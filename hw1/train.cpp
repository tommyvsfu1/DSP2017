#include "hmm.h"
#include <math.h>

int main( int argc, char* argv[] ) {
	// ./train iteration model_init.txt seq_model_01.txt model_01.txt
	int iteration = atoi(argv[1]);

	HMM model;
	loadHMM( &model, argv[2] );

	while ( iteration-- ) {

		double acc_init[MAX_STATE] = {0};
		double acc_epsilon[MAX_STATE][MAX_STATE] = {{0}};
		double acc_gamma[MAX_STATE] = {0};
		double acc_gamma_k[MAX_OBSERV][MAX_STATE] = {{0}};

		FILE *fp = open_or_die( argv[3], "r" );
		char buff[MAX_SEQ];
		int cnt = 0;
		
		while ( fscanf( fp, "%s", buff ) != EOF ) {

			int T = strlen(buff);
			cnt++;

			int data[MAX_SEQ];
			for ( int i = 0; i < T; i++ )
				data[i] = buff[i] - 'A';

			// calculate alpha
			double A[MAX_SEQ][MAX_STATE] = {{0}};
			for ( int t = 0; t < T; t++ ) {
				for ( int j = 0; j < model.state_num; j++ ) {
					if ( t == 0 ) {
						A[t][j] = model.initial[j] * model.observation[data[0]][j];
					}
					else {
						for ( int i = 0; i < model.state_num; i++ )
							A[t][j] += A[t-1][i] * model.transition[i][j];
						A[t][j] *= model.observation[data[t]][j];
					}
				}
			}

			// calculate beta
			double B[MAX_SEQ][MAX_STATE] = {{0}};
			for ( int t = T-1; t >= 0; t-- ) {
				for ( int i = 0; i < model.state_num; i++ ) {
					if ( t == T-1 ) B[T-1][i] = 1;
					else {
						for ( int j = 0; j < model.state_num; j++ )
							B[t][i] += model.transition[i][j] * model.observation[data[t+1]][j] * B[t+1][j];
					}
				}
			}

			// calculate gamma
			double prob = 0;
			for ( int i = 0; i < model.state_num; i++ )
				prob += A[T-1][i];

			double G[MAX_SEQ][MAX_STATE] = {{0}};
			for ( int t = 0; t < T; t++ )
				for ( int i = 0; i < model.state_num; i++ )
					G[t][i] = ( A[t][i] * B[t][i] ) / prob;

			// calculate epsilon
			double E[MAX_SEQ][MAX_STATE][MAX_STATE] = {{{0}}};
			for ( int t = 0; t < T-1; t++ ) {
				double numerator = 0;

				for ( int i = 0; i < model.state_num; i++ )
					for ( int j = 0; j < model.state_num; j++) {
						E[t][i][j] = A[t][i] * model.transition[i][j] * model.observation[data[t+1]][j] * B[t+1][j];
						numerator += E[t][i][j];
					}

				for ( int i = 0; i < model.state_num; i++ )
					for ( int j = 0; j < model.state_num; j++ )
						E[t][i][j] /= numerator;
			}

			// accumulate gamma & epsilon
			for ( int i = 0; i < model.state_num; i++ ) {
				acc_init[i] += G[0][i];
				for ( int t = 0; t < T; t++ ) {
					acc_gamma[i] += G[t][i];
					acc_gamma_k[data[t]][i] += G[t][i];
					for ( int j = 0; j < model.state_num; j++)
						acc_epsilon[i][j] += E[t][i][j];
				}
			}

		}
		fclose(fp);

		// recalculate parameters
		for ( int i = 0; i < model.state_num; i++ )
			model.initial[i] = acc_init[i] / cnt;

		for ( int i = 0; i < model.state_num; i++ )
			for ( int j = 0; j < model.state_num; j++ )
				model.transition[i][j] = acc_epsilon[i][j] / acc_gamma[i];

		for ( int i = 0; i < model.state_num; i++ )
			for ( int k = 0; k < model.observ_num; k++ )
				model.observation[k][i] = acc_gamma_k[k][i] / acc_gamma[i];

	}

	FILE *fp = open_or_die(argv[4], "w");
  	dumpHMM(fp, &model);
  	fclose(fp);

	return 0;
}