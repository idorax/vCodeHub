/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Exam 41 which is addressed from page#214 to page#216
 *
 * 1. ASSIGNMENT
 *    Input a sorted array A[] in ascending order and a number Z, please find
 *    all pairs of numbers in A[] as X, Y to satisfy X + Y = Z. If not found,
 *    return false, else return true.
 *
 *    e.g. A[] = {1, 2, 4, 7, 11, 15}, Z = 15
 *         Then we print {4,11}
 *
 * 2. INSTRUCTIONS
 *    Should not scan the array more than one time, that is, please make sure
 *    the time complexity is O(n).  It is not expected if the time complexity
 *    is O(n**2).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool
find(int a[], int n, int sum)
{
	bool found = false;

	int head = 0;
	int tail = n - 1;
	while (head < tail) {
		if (a[head] + a[tail] == sum) {
			found = true;
			printf("{%d,%d}\n", a[head], a[tail]);

			head++;
			tail--;
		} else {
			if (a[head] + a[tail] > sum)
				tail--;
			else
				head++;
		}
	}

	return found;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <sum>\n", argv[0]);
		return -1;
	}

	int sum = atoi(argv[1]);

	/* XXX: just use a hard-coded array to make life easy */
	int a[] = {1, 3, 4, 5, 7, 8, 9, 10};
	int n = sizeof(a) / sizeof(int);

	bool found = find(a, n, sum);

	return (found ? 0 : 1);
}
