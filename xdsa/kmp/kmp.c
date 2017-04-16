/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Knuth-Morris-Pratt Algorithm
 *
 * NOTES:
 *	Searches for occurrences of a pattern x within a main text string y
 *	by employing the simple observation: after a mismatch, the word
 *	itself allows us to determine where to begin the next match to
 *	bypass re-examination of previously matched characters.
 *
 *	o Preprecessing phase: O(m) space and time complexity
 *	o Searching     phase: O(n+m) time complexity (independent
 *	  from the alphabet size)
 *	o At most 2n - 1 character comparisons during the text scan
 *	o The maximum number of comparisons for a single text
 *	  character: <= log n M where n = (1 + 5**0.5)/2 is the golden ratio
 *
 *	The algorithm was invented in 1977 by Knuth and Pratt and
 *	independently by Morris, but the three published it jointly.
 *
 * REFERENCES:
 *	1. http://www-igm.univ-mlv.fr/~lecroq/string/node8.html
 *	2. http://www-igm.univ-mlv.fr/~lecroq/string/examples/exp8.html
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void
NEW_ARRAY(int **a, int m)
{
	*a = (int *)malloc(sizeof(int) * (m + 1));
}
#define DEL_ARRAY(p)	do {free(p);} while(0)
#define OUTPUT(n)	do {printf("%d\n", n);} while (0)

static void
preKmp(char *x, int m, int kmpNext[])
{
	int i, j;

	i = 0;
	j = kmpNext[0] = -1;
	while (i < m) {
		while (j > -1 && x[i] != x[j])
			j = kmpNext[j];

		i++;
		j++;
		if (x[i] == x[j]) {
			kmpNext[i] = kmpNext[j];
		} else {
			kmpNext[i] = j;
		}
	}
}

void
KMP(char *x, char *y)
{
	size_t m = strlen(x); /* x is pattern */
	size_t n = strlen(y); /* y is main text string */

	int *kmpNext; NEW_ARRAY(&kmpNext, m);

	/* --KMP Start-- */
	/* Preprocessing */
	preKmp(x, m, kmpNext);

	/* Searching */
	int i, j;
	i = j = 0;
	while (j < n) {
		while (i > -1 && x[i] != y[j])
			i = kmpNext[i];

		i++;
		j++;
		if (i >= m) {
			OUTPUT(j - i);
			i = kmpNext[i];
		}
	}
	/* --KMP End  -- */

	DEL_ARRAY(kmpNext);
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <text> <pattern>\n", argv[0]);
		fprintf(stderr, "e.g.   %s abc123abcdab abc\n", argv[0]);
		return -1;
	}

	char *text = argv[1];
	char *pattern = argv[2];
	KMP(pattern, text);

	return 0;
}
