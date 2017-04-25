/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Bucket Sort
 *
 * NOTE:
 *	Bucket sort(or bin sort), is a sorting algorithm that works by
 *	distributing the elements of an array into a number of buckets.
 *	Each bucket is then sorted individually, either using a different
 *	sorting algorithm, or by recursively applying the bucket sorting
 *	algorithm.
 *
 *	Typically, bucket sort works as follows:
 *	1. Set up an array of initially empty "buckets"
 *	2. Scatter: go over the original array, putting each object in
 *	            its bucket
 *	3. Sort each non-empty bucket
 *	4. Gather : visit the buckets in order and put all elements back
 *	            into the original array
 *
 *	Right here we just use insertion sorting algorithm to sort a single
 *	linked list.
 *
 *	In addition, we define N(=10) buckets, and use such hash algorithm in
 *	the following,
 *		a) get max number of a[] as MAX
 *		b) get width of the max number (i.e. MAX) as WIDTH
 *		   e.g. MAX = 9,   WIDTH = 1;
 *		        MAX = 99,  WIDTH = 2;
 *		        MAX = 999, WIDTH = 3;
 *		c) index = a[i] * N / (10 ** WIDTH)
 *	then we can dispatch a[i] to bucket[index]
 *
 * REFERENCES:
 *	1. http://www.cs.usfca.edu/~galles/visualization/BucketSort.html
 *	2. https://en.wikipedia.org/wiki/Bucket_sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum bool_s {false, true} bool_t;

bool_t g_isint = true;

typedef struct list_s {
	int data;
	struct list_s *next;
} list_t;

static void
list_init(list_t **head, list_t *node)
{
	if (*head == NULL) {
		*head = node;
		return;
	}

	list_t *q = *head;
	for (list_t *p = *head; p != NULL; p = p->next)
		q = p;
	q->next = node;
}

static void
list_show(list_t *head)
{
	if (head == NULL)
		return;

	for (list_t *p = head; p != NULL; p = p->next)
		printf("%d ", p->data);
	printf("\n");
}

static void
list_fini(list_t *head)
{
	list_t *p = head;
	while (p != NULL) {
		list_t *q = p;
		p = p->next;
		free(q);
	}
}

static void
list_insert(list_t **head, list_t *node)
{
	if (*head == NULL) {
		*head = node;
		return;
	}

	/* get both prev and next of the node to insert */
	list_t *node_prev = *head;
	list_t *node_next = NULL;
	for (list_t *p = *head; p != NULL; p = p->next) {
		if (p->data < node->data) {
			node_prev = p;
			continue;
		}

		node_next = p;
		break;
	}

	if (node_next == NULL) { /* append node to the tail */
		node_prev->next = node;
	} else {
		if (node_next == node_prev) { /* == *head */
			node->next = *head;
			*head = node;
			return;
		}

		/* node_prev -> node -> node_next */
		node_prev->next = node;
		node->next = node_next;
	}
}

static void
list_sort(list_t **head)
{
	if (*head == NULL)
		return;

	list_t *headp = *head; /* copy *head to headp before snip headp->next */
	list_t *p = headp->next; /* init p (move forward) before cut-off */
	headp->next = NULL;      /* now cut off headp->next */

	while (p != NULL) {
		list_t *this = p;  /* copy p to this before snip this->next */
		p = p->next;       /* move p forward before cut-off */
		this->next = NULL; /* now cut off this->next */
		list_insert(&headp, this); /* insert this node to list headp */
	}

	*head = headp; /* always save headp back even if headp == *head */
}

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

static int
get_hash_base(int a[], size_t n)
{
	/* get max one of a[] */
	int max = a[0];
	for (int i = 0; i < n; i++) {
		if (max < a[i])
		       max = a[i];
	}

	/* get hash base which is 10**N, N=1, 2, ... */
	int base = 1;
	for (int i = 0; i < get_width_of_num(max); i++)
		base *= 10;

	return base;
}

static void
scatter(list_t **bucket, size_t m, int a[], size_t n)
{
	int base = get_hash_base(a, n);

	for (int i = 0; i < n; i++) {
		/* 1. new a node for a[i] */
		list_t *nodep = NULL;
		nodep = (list_t *)malloc(sizeof (list_t));
		if (nodep == NULL) /* error: failed to malloc */
			return;

		nodep->data = a[i];
		nodep->next = NULL;

		/* 2. dispatch the new node to bucket[j] */
		int j = a[i] * m / base;
		list_init(&(bucket[j]), nodep);

		/* NOTE: dump bucket[j] just for visual observation */
		printf("%d:%d\t\t%d\tbucket[%d] : ", i, j, a[i], j);
		list_show(bucket[j]);
	}
}

static void
gather(list_t **bucket, size_t m, int a[], size_t n)
{
	int k = 0;
	for (int i = 0; i < m; i++) {
		if (bucket[i] == NULL)
			continue;

		for (list_t *p = bucket[i]; p != NULL; p = p->next) {
			a[k++] = p->data;

			if (k >= n) /* overflow */
				break;
		}

		list_fini(bucket[i]);
	}
}

void
bucketsort(int a[], size_t n)
{
	/* alloc bucket[] */
#define BUCKET_NUM 10
	list_t **bucket = (list_t **)malloc(sizeof (list_t *) * BUCKET_NUM);
	if (bucket == NULL) /* error: failed to malloc */
		return;
	for (int i = 0; i < BUCKET_NUM; i++)
		bucket[i] = NULL;

	/* scatter elements in a[] to bucket[] */
	scatter(bucket, BUCKET_NUM, a, n);

	/* walk bucket[] and sort */
	for (int i = 0; i < BUCKET_NUM; i++) {
		if (bucket[i] == NULL)
			continue;
		list_sort(&bucket[i]);

		/* NOTE: dump bucket[i] just for visual observation */
		printf("#after sort list#\tbucket[%d] : ", i);
		list_show(bucket[i]);
	}

	/* gather a[] by walking bucket[] */
	gather(bucket, BUCKET_NUM, a, n);

	free(bucket);
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
	bucketsort(a, n);
	printf("After  sorting: "); show(a, n);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
