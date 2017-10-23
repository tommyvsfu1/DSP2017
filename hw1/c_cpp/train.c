#include "hmm.h"
#include <math.h>

int main( int argc, char* argv[] ) {
	// ./train iteration model_init.txt seq_model_01.txt model_01.txt
	int iteration = atoi(argv[1]);

	HMM model;
	loadHMM( &model, argv[2] );

	int T = 500000;
	char data[T];
	// read training data

	while( iteration-- ) {

		// calculate alpha

		// calculate beta

		// calculate epsilon

		// calculate gamma

		// recalculate parameters

	}

	dumpHMM( stderr, &hmm_initial );

	return 0;
}