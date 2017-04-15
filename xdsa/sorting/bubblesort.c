/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Bubble Sort
 *
 * NOTES:
 *	The bubble sort makes multiple passes through a list. It compares
 *	adjacent items and exchanges those that are out of order. Each pass
 *	through the list places the next largest value in its proper place.
 *	In essence, each item "bubbles" up to the location where it belongs.
 *
 * REFERENCES:
 *	1. https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
 *	2. https://interactivepython.org/courselib/static/pythonds/SortSearch/TheBubbleSort.html
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

void
bubblesort(int a[], size_t n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			printf("#i=%d, j=%d:\t", i, j);   show(a, n);

			if (a[j+1] < a[j]) {
				exchange(a, j+1, j);

				printf("\t<-- swap(a[%d], a[%d])", j + 1, j);
			}

			printf("\n");
		}
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
	bubblesort(a, n);
	printf("After  sorting: "); show(a, n); printf("\n");

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
