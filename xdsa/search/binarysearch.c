/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Binary search also known as half-interval search, logarithmic search, or
 * binary chop, is a search algorithm that finds the position of a target value
 * within a sorted array. Binary search compares the target value to the middle
 * element of the array; if they are unequal, the half in which the target
 * cannot lie is eliminated and the search continues on the remaining half until
 * it is successful. If the search ends with the remaining half being empty, the
 * target is not in the array.
 *
 * Worst-case performance : O(logN)
 * Best-case  performance : O(1)
 * Average    performance : O(logN)
 * Worst-case space complexity : O(1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void
show(int a[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%-2d ", a[i]);
	printf("\n");
}

static void
bubblesort(int a[], int n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			if (a[j+1] < a[j]) {
				int t = a[j];
				a[j] = a[j+1];
				a[j+1] = t;
			}
		}
	}
}

int
binary_search(int a[], int n, int num)
{
	int left = 0;
	int right = n - 1;
	int m = 0;

	while (1) {
		m = (left + right) / 2;

		if (a[m] == num)
			return m;
		else if (a[m] < num)
			left = m + 1;
		else /* a[m] > num */
			right = m - 1;

		if (right < left)
			break;
	}

	return -1; /* not found */
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <NUM> <N1> [N2] ...\n", argv[0]);
		return -1;
	}

	int num = atoi(argv[1]);

	argc -= 2;
	argv += 2;

	int n = argc;
	int *a = (int *)malloc(sizeof(int) * n);
#define VALIDATE(p) do { if (p == NULL) return -1; } while (0)
	VALIDATE(a);

	for (int i = 0; i < n; i++)
		*(a+i) = atoi(argv[i]);

	printf("Before sorting: "); show(a, n);
	bubblesort(a, n);
	printf("After  sorting: "); show(a, n);

	int idx = binary_search(a, n, num);
	if (idx != -1)
		printf("a[%d] = %d\n", idx, num);
	else
		fprintf(stderr, "not found %d in a[]\n", num);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
