/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Radix Sorting based on LSD (Least Significant Digit first)
 *
 * NOTE:
 *     1. Only unsigned int is supported;
 *     2. This radix sorting algorithm walks per digit of an integer by
 *        decimal bit.
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
	printf("\n");
}

/*
 * Get the digit of number by index(= 0, 1, 2, ...)
 *
 * e.g.
 *     num = 6543210
 *     ------+------
 *     index | digit
 *     ------+------
 *         0 | 0
 *         1 | 1
 *         2 | 2
 *         3 | 3
 *         4 | 4
 *         5 | 5
 *         6 | 6
 *         7 | 0
 *        .. | ..
 *     ------+------
 */
static unsigned char
get_digit_byindex(int num, unsigned int index)
{
	int q = 0; /* quotient */
	int r = 0; /* remainder */
	for (int i = index; i >= 0; i--) {
		r = num % 10;
		q = num / 10;
		num = q;
	}
	return (unsigned char)r;
}

/*
 * Get width of a number
 * e.g.
 *   for i in [  0 .. 9  ] // width = 1
 *   for i in [ 10 .. 99 ] // width = 2
 *   for i in [100 .. 999] // width = 3
 *   ...
 */
static int
get_width_of_num(int num)
{
	int w = 1;
	for (int q = num / 10; q != 0; q /= 10)
		w++;
	return w;
}

/*
 * Build bin[] by index(=0, 1, ..., N-1) (N is the max width of num in a[])
 *
 * NOTE: This is the core function to understand radix sorting based on LSD
 */
static void
build_bin_byindex(int bin[], size_t nb, int a[], size_t na, int index)
{
	/* 1. always reset bin[] */
	for (int i = 0; i < nb; i++)
		bin[i] = 0;

	/* 2. init bin[] by walking a[] */
	for (int i = 0; i < na; i++) {
		unsigned char d = get_digit_byindex(a[i], index);
		bin[d]++;
	}

	/* NOTE: dump bin[] just for visual observation */
	printf("1#bin[]:\t"); show(bin, nb);

	/* 3. build bin[] */
	for (int i = 1; i < nb; i++)
		bin[i] += bin[i-1];

	/* NOTE: dump bin[] just for visual observation */
	printf("2#bin[]:\t"); show(bin, nb);
}

void
radixsort(int a[], int n)
{
	/* get the max width of num in a[] */
	int max = a[0];
	for (int i = 0; i < n; i++) {
		if (a[i] > max)
			max = a[i];
	}
	int maxwidth = get_width_of_num(max);

	/* alloc bin[] to store the number of per digit */
	int bin[10] = { 0 };

	/* alloc aux[] to save a[] while rebuilding a[] */
	int *aux = (int *)malloc(sizeof(int) * n);
	if (aux == NULL) /* error */
		return;

	/* LSD (Least Significant Digit first) */
	for (int index = 0; index < maxwidth; index++) {
		/* 1. build bin[] */
		build_bin_byindex(bin, 10, a, n, index);

		/* 2. copy a[] to aux[] */
		for (int i = 0; i < n; i++)
			aux[i] = a[i];

		/* 3. rebuild a[] */
		for (int i = n - 1; i >= 0; i--) {
			unsigned char d = get_digit_byindex(aux[i], index);
			int j = bin[d] - 1;
			a[j] = aux[i];
			bin[d]--;
		}

		/* NOTE: dump a[] for visual observation */
		printf("%dth bit(done): \t", index); show(a, n);
	}

	free(aux);
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

	printf("Before sorting: "); show(a, n);
	radixsort(a, n);
	printf("After  sorting: "); show(a, n);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
