# include <stdio.h>
# include <string.h>

#include "option.h"
#include "File.h"
#include "Ngram.h"
#include "Vocab.h"
#include "VocabMap.h"
#include "Prob.h"

// Usage: ./disambig -text $file -map $map -lm $LM -order $order

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

	Vocab vocab, Big5, ZhuYin;
	VocabMap map(ZhuYin, Big5);
	map.read(mapFile);
	mapFile.close();

	Ngram lm(vocab, order);
	lm.read(lmFile);
	lmFile.close();

	char *line;
	while( line = textFile.getline() ) {

		VocabString sen[maxWordsPerLine];
		// parse a line into Chinese char (seperate by space)
		unsigned len = Vocab::parseWords(line, sen[1], maxWordsPerLine) + 2;

		LogP prob[len][1024];
		int candi_len[len];
		VocabIndex candi[len][1024];	// index of Big5
		VocabIndex backtrack[len][1024];

		// Initialize
		sen[0] = "<s>";
		sen[len-1] = "</s>";
		prob[0][0] = {{0.0}};

		// Store all the candidate's index 
		for ( int i = 1; i < len; i++ ) {
			VocabIndex index;	Prob p;		int cnt = 0;
			VocabMapIter iter(map, ZhuYin.getIndex(sen[i]));
			iter.init();
			while( iter.next(index, p) ) {
				candi[i][cnt] = index;
				cnt++;
			}
			candi_len[i] = cnt;
		}

		// do Viterbi recursion
		for( int i = 1; i < len; i++ ) {
			// check all candidates of sen[i]
			for( int j = 0; j < candi_len[i]; j++ ) {
				LogP max = 0;
				// check all candidates of sen[i-1]
				for( int k = 0; k < candi_len[i-1]; k++) {
					VocabString wj = Big5.getWord(candi[i][j]);
					VocabString wk = Big5.getWord(candi[i-1][k]);
					// Calculate P(sen[i]|sen[i+1])
					LogP tmp = prob[i-1][k] + lm.wordProb( wj, wk );
					if ( tmp > max ) {
						max = tmp;
						backtrack[i][j] = candi[i-1][k];
					}
				}
				prob[i][j] = max;
			}
		}

		// Find the max path

		// Trace back

		// Add <s> </s> and print

	}

	textFile.close();

	return 0;
}