# include <stdio.h>
# include <string.h>

#include "option.h"

/* Usage:																*/
/* ./disambig –text $file –map $map –lm $LM –order $order > $output		*/
/* argv[0]	  1     2     3    4    5   6   7      8					*/

static unsigned order = 2;
static char *lmFile = 0;
static char *mapFile = 0;
static char *textFile = 0;

static Option options[] = {
    { OPT_STRING, "lm", &lmFile, "hidden token sequence model" },
    { OPT_UINT, "order", &order, "ngram order to use for lm" },
    { OPT_STRING, "map", &mapFile, "mapping from observable to hidden tokens" },
    { OPT_STRING, "text", &textFile, "text file to disambiguate" },
};

int main ( int argc, char **argv ) {

	// parse options
	Opt_Parse(argc, argv, options, 4, 0);

	printf("text: %s, map: %s, lm: %s, order: %d\n", textFile, mapFile, lmFile, order);

	// read in files: text, map(candidate), lm(prob), order(2 for bigram)

	// for every sentence, read the map and list the candidate

	// do Viterbi

	// Add <s> </s> and print


	return 0;
}