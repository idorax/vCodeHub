/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Radix Sort based on LSD (Least Significant Digit first)
 *
 * NOTE:
 *     1. Only unsigned int is supported;
 *     2. This radix sorting algorithm walks per digit of an integer by
 *        binary bit, hence the total loops is 32 at most.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum bool_s {false, true} bool_t;

bool_t g_isint = true;

typedef struct bin_unit_s {
	size_t len;
	int *unit;
} bin_unit_t;

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

static bool_t
is_sorted(int a[], size_t n)
{
	for (int i = 1; i < n; i++)
		if (a[i] < a[i-1])
			return false;
	return true;
}

/*
 * Get the digit of number by index(= 0, 1, 2, ...)
 */
static unsigned char
get_digit_byindex(int num, unsigned int index)
{
	return ((unsigned int)num >> index) & 0x1;
}

/*
 * Build bin[] by index(=0, 1, ..., N-1) (N is the max width of num in a[])
 *
 * NOTE: This is the core function to understand radix sorting based on LSD
 */
static void
build_bin_byindex(bin_unit_t *bin[], size_t nb, int a[], size_t na, int index)
{
	/* 1. always reset bin[][] */
	for (int i = 0; i < nb; i++) {
		bin[i]->len = 0;
		for (int j = 0; j < na; j++)
			(bin[i]->unit)[j] = 0x0;
	}

	/* 2. init bin[][] by walking a[] */
	for (int i = 0; i < na; i++) {
		unsigned char d = get_digit_byindex(a[i], index);
		(bin[d]->unit)[bin[d]->len] = a[i];
		(bin[d]->len)++;

		/* NOTE: print a[i] and Xth bit just for visual observation */
		printf("%dth bit of\t%d\t\tis %d\n", index, a[i], d);
	}

	/* NOTE: dump bin[] just for visual observation */
	for (int i = 0; i < nb; i++) {
		printf("bin[%d]:\t\t", i);
		show(bin[i]->unit, bin[i]->len);
	}
}

/*
 * Rebuild a[] by walking numbers saved in (bin[0]->unit)[] and
 *                                         (bin[1]->unit)[]
 *
 * e.g.
 *        bin[0] = { .len = 2, .unit = {4, 2}    }
 *        bin[1] = { .len = 3, .unit = {5, 3, 1} }
 *          a[5] = { 4, 2, 5, 3, 1 }
 *
 *        bin[0] bin[1] ===> a[5]
 *
 *                             1
 *                             3
 *                 1           5
 *          2      3           2
 *          4      5    ===>   4
 *        -----  -----       -----
 *          0      1
 */
static void
rebuild(int a[], size_t na, bin_unit_t *bin[], size_t nb)
{
	int k = 0;
	for (int i = 0; i < nb; i++)
		for (int j = 0; j < bin[i]->len; j++)
			a[k++] = (bin[i]->unit)[j];
}

void
radixsort(int a[], int n)
{
	/* alloc aux[] for puting a[] to (bin[i]->unit)[] (i= 0, 1) */
	int *aux = (int *)malloc(sizeof(int) * n * 2);
	if (aux == NULL) /* error */
		return;

	/*
	 * alloc bin[2], (hence, worst space complexity is O(n*2 + 2))
	 * if number M of a[] whose Xth digit is 0, save M to (bin[0]->unit)[]
	 * else                                     save M to (bin[1]->unit)[]
	 *                    where X = 0, 1, ..., 31
	 */
	bin_unit_t bin0 = {.len = 0, .unit = aux + 0};
	bin_unit_t bin1 = {.len = 0, .unit = aux + n};
	bin_unit_t *bin[2] = {&bin0, &bin1};

	/* LSD (Least Significant Digit first) */
	for (int index = 0; index < 32; index++) {
		/* 0. break if a[] is in order */
		if (is_sorted(a, n))
			break;

		/* 1. build bin[] */
		build_bin_byindex(bin, 2, a, n, index);

		/* 2. rebuild a[] by walking bin[] */
		rebuild(a, n, bin, 2);

		/* NOTE: dump a[] for visual observation */
		printf("%dth bit(done):\t", index); show(a, n);
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
