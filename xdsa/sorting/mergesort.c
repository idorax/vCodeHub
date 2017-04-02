/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Top-down Merge Sort (2-way)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum boolean_s {false, true} boolean_t;

boolean_t g_isint = false;

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
	printf("\n");
}

/*
 * Merge two sorted src list A[] and B[] to dst list C[]
 */
static void
merge2to1(int c[],  size_t nc, int a[], size_t na, int b[], size_t nb)
{
	if (nc < na + nb) /* error */
		return;

	int i = 0; /* walk list A : read  */
	int j = 0; /* walk list B : read  */
	int k = 0; /* walk list C : write */
	while (i < na && j < nb) {
		if (a[i] < b[j])
			c[k++] = a[i++];
		else
			c[k++] = b[j++];
	}

	while (i < na)
		c[k++] = a[i++];

	while (j < nb)
		c[k++] = b[j++];
}

/*
 * Merge a[l..m] and a[m+1..r] to a[l..r]
 *    l: left   : 0 <= l <= n-1
 *    m: middle : 0 <= m <= n-1
 *    r: right  : 0 <= r <= n-1
 *                l <= m <= r
 */
static void
merge(int a[], size_t n, int l, int m, int r)
{
	/*
	 * NOTE: To get better performance, we can malloc aux[]
	 *       out of this function, and just repeat using it
	 */
	int *aux = (int *)malloc(sizeof(int) * n);
	if (aux == NULL) /* error */
		return;

	for (int i = 0; i < n; i++)	// copy a[] to aux[]
		aux[i] = a[i];

	for (int i = l; i <= r; i++)	// erase a[l..r] on purpose
		a[i] = 0xfeedfeed;	// just for better debugging

	int *dc    = a + l;
	size_t ndc = n - l;
	int *sa    = aux + l;		// src list a[l .. m]
	size_t nsa = m - l + 1;		//          len = m - l + 1
	int *sb    = aux + m + 1;	// src list b[m+1 .. r]
	size_t nsb = r - m;		//          len = r - (m+1) + 1 = r - m
	merge2to1(dc, ndc, sa, nsa, sb, nsb);

	free(aux);
}

void
td_mergesort(int a[], size_t n, int left, int right)
{
	if (left >= right)
		return;

	int middle = (left + right) / 2;
	td_mergesort(a, n, left, middle);	// make sure left is sorted
	td_mergesort(a, n, middle+1, right);	// make sure right is sorted
	merge(a, n, left, middle, right);	// merge a[l..m] and a[m+1..r]
}

#define MERGESORT(a, n) td_mergesort(a, n, 0, n-1)

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

	printf("Before sorting: "); show(a, n);
	MERGESORT(a, n);
	printf("After  sorting: "); show(a, n);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
