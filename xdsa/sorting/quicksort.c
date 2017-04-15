/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Quick Sort
 *
 * NOTES:
 *	The quick sort uses divide and conquer to gain the same advantages as
 *	the merge sort, while not using additional storage. As a trade-off,
 *	however, it is possible that the list may not be divided in half.
 *	When this happens, we will see that performance is diminished.
 *
 *	A quick sort first selects a value, which is called the pivot value.
 *	Although there are many different ways to choose the pivot value, we
 *	will simply use the first item in the list. The role of the pivot value
 *	is to assist with splitting the list. The actual position where the
 *	pivot value belongs in the final sorted list, commonly called the split
 *	point, will be used to divide the list for subsequent calls to the
 *	quick sort.
 *
 * REFERENCES:
 *	1. https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
 *	2. https://interactivepython.org/courselib/static/pythonds/SortSearch/TheQuickSort.html
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
partition(int a[], size_t n, int left, int right)
{
	/* left and right scan indices */
	int i = left + 1;
	int j = right;

	/* get partitioning item i.e. pivot */
	int pivot = a[left];

	/*
	 * scan right, scan left, check for scan complete, and exchange
	 */
	while (1) {
		for (; j > left; j--) { /* scan right */
			if (a[j] < pivot)
				break;
		}

		for (; i < right; i++) { /* scan left */
			if (a[i] > pivot)
				break;
		}

		if (i >= j)
			break;

		printf("#%d:%d\t\t", left, right); show(a, n);
		printf("\t<-- swap(a[%x], a[%x])\n", i, j);

		exchange(a, i, j);
	}

	/*
	 * put pivot = a[j] into the right position, with
	 *      a[left .. j-1] <= a[j] <= a[j+1 .. right]
	 */
	printf("#%d:%d\t\t", left, right); show(a, n);
	printf("\t<-- swap(a[%x], a[%x])\t next pivot : a[%x]\n", left, j, j);

	exchange(a, left, j);

	return j;
}

void
quicksort(int a[], size_t n, int left, int right)
{
	if (right <= left)
		return;

	int j = partition(a, n, left, right); /* PARTITION */
	quicksort(a, n, left,  j - 1); /* sort left  part a[left .. j-1]   */
	quicksort(a, n, j + 1, right); /* sort right part a[j+1  .. right] */
}

#define QSORT(a, n)     quicksort(a, n, 0, n - 1)

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
	QSORT(a, n);
	printf("After  sorting: "); show(a, n); printf("\n");

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
