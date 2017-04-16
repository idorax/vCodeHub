/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Simple Selection Sort
 *
 * NOTES:
 *	The simple selection sort improves on the bubble sort by making only one
 *	exchange for every pass through the list. In order to do this,
 *	a selection sort looks for the smallest value as it makes a pass and,
 *	after completing the pass, places it in the proper location.  After the
 *	first pass, the smallest item is in the correct place. After the second
 *	pass, the next smallest is in place. This process continues and requires
 *	n-1 passes to sort n items, since the final item must be in place after
 *	the (n-1) st pass.
 *
 * REFERENCES:
 *	1. https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
 *	2. https://interactivepython.org/courselib/static/pythonds/SortSearch/\
 *	   TheSelectionSort.html
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

static void
exchange(int a[], int i, int j)
{
	int t = a[i];
	a[i] = a[j];
	a[j] = t;
}

static int
getIndexOfMin(int a[], size_t n)
{
	int min = 0;
	for (int i = 0; i < n; i++)
		if (a[i] < a[min])
			min = i;
	return min;
}

/*
 * Simple Selection Sort (s2sort in short)
 *
 * NOTE:
 *	a[0 .. i] is always sorted and
 *	a[i+1 .. n-1] is unsorted
 */
void
s2sort(int a[], size_t n)
{
	for (int i = 0; i < n; i++) {
		printf("i=%2d:\t\t", i); show(a, n);

		int min = getIndexOfMin(a+i, n-i) + i;
		printf("\t<-- min = %x", min);

		if (i != min) {
			exchange(a, i, min);

			printf(" : exchange(a[%x], a[%x])", i, min);
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
	s2sort(a, n);
	printf("After  sorting: "); show(a, n); printf("\n");

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
