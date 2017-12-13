# include <stdio.h>
# include <string.h>

#include "option.h"
#include "File.h"
#include "Ngram.h"
#include "Vocab.h"
#include "VocabMap.h"

/* Usage:																*/
/* ./disambig -text $file -map $map -lm $LM -order $order > $output		*/

static unsigned order = 2;
static char *lmFileName = 0;
static char *mapFileName = 0;
static char *textFileName = 0;

static Option options[] = {
    { OPT_STRING, "lm", &lmFileName, "hidden token sequence model" },
    { OPT_STRING, "map", &mapFileName, "mapping from observable to hidden tokens" },
    { OPT_STRING, "text", &textFileName, "text file to disambiguate" },
    { OPT_UINT, "order", &order, "ngram order to use for lm" },
};

int main ( int argc, char **argv ) {

	// parse options
	Opt_Parse(argc, argv, options, 4, 0);

	// read in files: text, map(candidate), lm(prob), order(2 for bigram)
	File lmFile(lmFileName, "r");
	File mapFile(mapFileName, "r");
	File textFile(textFileName, "r");

	Vocab Big5, ZhuYin;
	VocabMap map(ZhuYin, Big5);
	map.read(mapFile);
	mapFile.close();

	Ngram lm(ZhuYin, order);
	lm.read(lmFile);
	lmFile.close();

	char *line;
	while( line = textFile.getline() ) {

		// list the candidate

		// do Viterbi

		// Trace back

		// Add <s> </s> and print


	}
	textFile.close();

	return 0;
}