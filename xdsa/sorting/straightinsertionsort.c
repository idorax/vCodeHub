/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Straight Insertion Sort
 *
 * NOTES:
 *	Straight insertion sort always maintains a sorted sublist in the lower
 *	positions of the list. Each new item is then "inserted" back into the
 *	previous sublist such that the sorted sublist is one item larger.
 *
 *	We begin by assuming that a list with one item (position 0) is already
 *	sorted. On each pass, one for each item 1 through n-1, the current item
 *	is checked against those in the already sorted sublist. As we look back
 *	into the already sorted sublist, we shift those items that are greater
 *	to the right. When we reach a smaller item or the end of the sublist,
 *	the current item can be inserted.
 *
 * REFERENCES:
 *	1. https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
 *	2. https://interactivepython.org/courselib/static/pythonds/SortSearch/\
 *	   TheInsertionSort.html
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
#define NEWLINE	"" /* default is '\n' */
	printf("%s", NEWLINE);
}

/*
 * Insert a[n] before a[m]
 *                                                .-----------.
 *                                                |           |
 * o Input : a[m-1], a[m], a[m+1], ..., a[n-1], a[n], a[n+1]  |
 *                        \     \             \               |
 * o Output: a[m-1], a[n], a[m], a[m+1], ..., a[n-1], a[n+1]  |
 *                       \____________________________________/
 */
static void
insert(int a[], int m, int n)
{
	int t = a[n];
	for (int i = n; i > m; i--)
		a[i] = a[i-1];
	a[m] = t;
}

/*
 * Straight Insertion Sort (sisort in short)
 *
 * NOTES:
 *	1. a[i .. n-1] is not sorted and
 *	   a[0 .. i-1] is sorted
 *	2. walk a[0 .. i-1], if a[i] < a[j], insert a[i] before a[j]
 */
void
sisort(int a[], size_t n)
{
	for (int i = 1; i < n; i++) {
		printf("#%d:\t\t", i);  show(a, n);

		for (int j = 0; j < i; j++) {
			if (a[i] < a[j]) {
				printf("\t<-- insert a[%x] before a[%x]", i, j);

				insert(a, j, i);
				break;
			}
		}

		printf("\n");
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
	sisort(a, n);
	printf("After  sorting: "); show(a, n); printf("\n");

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
