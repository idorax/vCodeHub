/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Assume the size of an array is 9, e.g.
 *
 * Array: a[0] a[1] a[2] a[3] a[4] a[5] a[6] a[7] a[8] // n = 9
 *
 * Heap :        a[0]               --- Level 0 ---
 *                / \
 *               /   \
 *              /     \
 *            a[1]    a[2]          --- Level 1 ---
 *            / \      / \
 *           /   \    /   \
 *         a[3] a[4] a[5] a[6]      --- Level 2 ---
 *         /  \
 *       a[7] a[8]                  --- Level 3 ---
 *
 * For node a[i],  i = 0, 1, 2, ..., N-1
 *
 *       Index of its Left  Child = (i + 1) * 2 - 1 = i * 2 + 1
 *       Index of its Right Child = (i + 1) * 2     = i * 2 + 2
 *       Index of its Parent      = (i + 1) / 2 - 1 = (i - 1) / 2
 *
 * C code:
 *
 *       int getIndexOfLeftChild(i)  { return i * 2 + 1;         }
 *       int getIndexOfRightChild(i) { return i * 2 + 2;         }
 *       int getIndexParent(i)       { return (i - 1) / 2;       }
 *       int hasLeftChild(n, i)      { return ((i * 2 + 1) < n); }
 *       int hasRightChild(n, i)     { return ((i * 2 + 2) < n); }
 *       int hasParent(i)            { return (i > 0);           }
 *       int isLeafNode(n, i)        { return (i >= n / 2);      }
 */

static void
show(int a[], size_t n)
{
	for (int i = 0; i < n; i++)
		printf("%-2c ", a[i]);
	printf("\n");
}

static void
exchange(int a[], int i, int j)
{
	int t = a[i];
	a[i] = a[j];
	a[j] = t;
}

static int
getIndexOfParent(int i)
{
	return (i - 1) / 2;
}

static int
getIndexOfLeftChild(int i)
{
	return i * 2 + 1;
}

static int
getIndexOfRightChild(int i)
{
	return i * 2 + 2;
}

static void
swim(int a[], size_t n, int k)
{
#define ISNOTROOT(k)    (k != 0)
	while (ISNOTROOT(k)) {
		int parent = getIndexOfParent(k);
		if (a[k] <= a[parent])
			break;

		/* swim only if a[k] > a[parent] */
		exchange(a, k, parent);
		k = parent;
	}
}

static void
sink(int a[], size_t n, int k)
{
#define ISLEAFNODE(n, k) (k >= n / 2)
	while (!ISLEAFNODE(n, k)) {
		int left  = getIndexOfLeftChild(k);
		int right = getIndexOfRightChild(k);

		int child = -1; /* set to invalid index on purpose */
		if (left < n && right < n)
			child = (a[left] > a[right]) ? left : right;
		else if (left < n && right >= n)
			child = left;
		else if (left >= n && right < n)
			child = right;
		else
			child = k;

		if (a[k] >= a[child])
			break;

		/* sink only if max of children of a[k] say a[child] > a[k] */
		exchange(a, k, child);
		k = child;
	}
}

static void
constructMaxHeap(int a[], size_t n)
{
	for (int i = 0; i < n; i++)
		swim(a, i, i);
}

void
heapsort(int a[], size_t n)
{
	constructMaxHeap(a, n);

	while (n > 0) {
		exchange(a, 0, n - 1);

		n--;
		sink(a, n, 0);	// NOTE: "constructMaxHeap(a, n);" also works
	}			//       but the performance would be low as
}				//       sink() is not used

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

	for (int i = 0; i < n; i++)
		*(a+i) = argv[i][0];

	printf("                ");
	for (int i = 0; i < n; i++)
		printf("%-2d ", i);
	printf("\n");

	printf("Before sorting: "); show(a, n);
	heapsort(a, n);
	printf("After  sorting: "); show(a, n);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);

	return 0;
}
