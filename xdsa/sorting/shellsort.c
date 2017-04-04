/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Shell Sort based on Shell's original gap sequence
 *
 * Here are some common gap sequences in the following,
 *	1. Shell's original sequence: N/2, ..., 4, 2, 1 (repeatedly divide by 2)
 *	2. Hibbard's      increments: 2**k-1, ..., 15,  7, 3, 1
 *	3. Knuth's        increments: ..., 121,    40, 13, 4, 1
 *	4. Sedgewick's    increments: ..., 109,    41, 19, 5, 1
 * For more, please refer to:
 *	https://en.wikipedia.org/wiki/Shellsort#Gap_sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum bool_s {false, true} bool_t;

bool_t g_isint = true;

static void
show(int a[], size_t n)
{
	if (g_isint) {
		for (int i = 0; i < n; i++)
			printf("%-2d ", a[i]);
	} else {
		for (int i = 0; i < n; i++)
			printf("%-2c ", a[i]);
	}
}

static void
exchange(int a[], int i, int j)
{
	int t = a[i];
	a[i] = a[j];
	a[j] = t;
}

void
shellsort(int a[], size_t n)
{
	int h = 1;
	while (h < n / 2)
		h = 2 * h; // 1, 2, 4, ... [N/2]

	while (h >= 1) {
		for (int i = h; i < n; i++) {
			for (int j = i; j >= h && (a[j] < a[j-h]); j -= h) {
				exchange(a, j, j-h);

				/* NOTE: dump a[] just for observation */
				printf("\t\t");
				show(a, n);
				printf("\t<--exchanged(a[%d], a[%d])\n",
				    j, j-h);
				/* NOTE: END */
			}
		}

		/* NOTE: dump a[] just for observation */
		printf("#h=%d\t\t", h);
		show(a, n);
		printf("\tDONE\n");
		/* NOTE: END */

		h /= 2;
	}
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <C1> [C2] ...\n", argv[0]);
		return -1;
	}

	argc--;
	argv++;

	int n = argc;
	int *a = (int *)malloc(sizeof(int) * n);
#define VALIDATE(p) do { if (p == NULL) return -1; } while (0)
	VALIDATE(a);

	char *s = getenv("ISINT");
	if (s != NULL && strncmp(s, "true", 4) == 0)
		g_isint = true;
	else if (s != NULL && strncmp(s, "false", 4) == 0)
		g_isint = false;

	if (g_isint) {
		for (int i = 0; i < n; i++)
			*(a+i) = atoi(argv[i]);
	} else {
		for (int i = 0; i < n; i++)
			*(a+i) = argv[i][0];
	}

	printf("                ");
	for (int i = 0; i < n; i++)
		printf("%-2x ", i);
	printf("\n");

	printf("Before sorting: "); show(a, n); printf("\n");
	shellsort(a, n);
	printf("After  sorting: "); show(a, n); printf("\n");

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
