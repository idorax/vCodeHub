/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Singly Linked List Sorting
 *
 *	1. convert a[] to a singly linked list L
 *	   e.g. int a[] = {9, 8, 7, 7}
 *	   ==>        L = 9->8->7->7->NULL
 *	2. sort list L via straight insertion sorting
 *	   ==>        L = 7->7->8->9->NULL
 *	3. convert list L back to a[]
 *	   ==>      a[] = {7, 7, 8, 9}
 *
 * Note there are two solutions,
 *	(a) Use aux[] to save addr of per node in list L, then sort aux[] via
 *	    straight insertion sorting algorithm, and rebuild list L by walking
 *	    sorted aux[]
 *	(b) Don't use aux[] but directly sort the single liked list L
 *
 *	To implement the two solutions above, (a) is very easy and (b) is a bit
 *	difficult. Please also keep in mind that performance of (b) is good.
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

#ifdef _USE_AUX /* solution (a) */
static void
list_init(list_t **head, list_t *node)
{
	static list_t *tail = NULL;

	if (*head == NULL) {
		*head = node;
		tail = node;
		return;
	}

	tail->next = node;
	tail = node;
}
#else
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
#endif

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

#ifdef _USE_AUX /* solution (a) */
/*
 * Insert a[n] before a[m]
 *                                                .-----------.
 *                                                |           |
 * o Input : a[m-1], a[m], a[m+1], ..., a[n-1], a[n], a[n+1]  |
 *                        \     \             \               |
 * o Output: a[m-1], a[n], a[m], a[m+1], ..., a[n-1], a[n+1]  |
 *                       \____________________________________/
 */
static void
insert(list_t *a[], int m, int n)
{
	list_t *t = a[n];
	for (int i = n; i > m; i--)
		a[i] = a[i-1];
	a[m] = t;
}

/*
 * Straight Insertion Sort (sisort in short)
 *
 * NOTES:
 *	1. a[i .. n-1] is not sorted and
 *	   a[0 .. i-1] is sorted
 *	2. walk a[0 .. i-1], if a[i] < a[j], insert a[i] before a[j]
 */
static void
sisort(list_t *a[], size_t n)
{
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (a[i]->data < a[j]->data) {
				insert(a, j, i);
				break;
			}
		}
	}
}

static void
list_sort(list_t **head)
{
	if (head == NULL || *head == NULL)
		return;

	/* get total number of nodes in the singly linked list */
	int len = 0;
	for (list_t *p = *head; p != NULL; p = p->next)
		len++;

	/* malloc aux[] */
	list_t **aux = (list_t **)malloc(sizeof (list_t *) * len);
	if (aux == NULL) /* error */
		return;

	/* save addr of per node to aux[] */
	int k = 0;
	for (list_t *p = *head; p != NULL; p = p->next)
		aux[k++] = p;

	/* sort aux[] via straight insertion sorting algorithm */
	sisort(aux, len);

	/* rebuild the singly linked list by walking aux[] */
	*head = aux[0];
	for (int i = 0; i < len - 1; i++)
		aux[i]->next = aux[i+1];
	aux[len-1]->next = NULL;

	free(aux);
}
#else
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
#endif

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

static void
a2l_init(list_t **head, int a[], size_t n)
{
	for (int i = 0; i < n; i++) {
		list_t *nodep = NULL;
		nodep = (list_t *)malloc(sizeof (list_t));
		if (nodep == NULL) /* error: failed to malloc */
			return;

		nodep->data = a[i];
		nodep->next = NULL;

		list_init(head, nodep);

		printf("Append a[%x]=%d:\t", i, nodep->data); list_show(*head);
	}

}

static void
a2l_fini(list_t *head, int a[], size_t n)
{
	if (head == NULL)
		return;

	int k = 0;
	for (list_t *p = head; p != NULL; p = p->next) {
		a[k++] = p->data;

		if (k >= n)
			break;
	}

	list_fini(head);
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
	list_t *head = NULL;
	a2l_init(&head, a, n);
	list_sort(&head);
	a2l_fini(head, a, n);
	printf("After  sorting: "); show(a, n);

#define FREE(p) do { free(p); p = NULL; } while (0)
	FREE(a);
	return 0;
}
