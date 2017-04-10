/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * This small program is to reverse every k nodes of a single linked list.
 * Note if the list size is not a multiple of k, then leave the remainder
 * nodes as is.
 *
 * Example:
 *        Inputs:      1->2->3->4->5->6->7->8->NULL
 *        Outputs: (a) k = 3
 *                     3->2->1->6->5->4->7->8->NULL
 *                 (b) k = 4
 *                     4->3->2->1->8->7->6->5->NULL
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct list_s {
	int data;
	struct list_s *next;
} list_t;

typedef unsigned long uintptr_t;

static void
show(list_t *head)
{
	if (head == NULL)
		return;

	for (list_t *p = head; p != NULL; p = p->next)
		printf("%d ", p->data);
	printf("\n");
}

static void
fini(list_t *head)
{
	list_t *p = head;
	while (p != NULL) {
		list_t *q = p;
		p = p->next;
		free(q);
	}
}

static void
init(list_t **head, char *a[], int n)
{
	list_t *new = NULL;
	list_t *cur = NULL;

	for (int i = 0; i < n; i++) {
		new = (list_t *)malloc(sizeof (list_t));
		if (new == NULL) /* error: failed to malloc */
			return;

		new->data = atoi(a[i]);
		new->next = NULL;

		if (*head == NULL)
			*head = new;
		else
			cur->next = new;
		cur = new;
	}
}

static int
get_length(list_t *head)
{
	int len = 0;
	for (list_t *p = head; p != NULL; p = p->next)
		len++;
	return len;
}

static void
reverse_array(uintptr_t a[], int n)
{
	for (int i = 0; i < n / 2; i++) {
		uintptr_t t = a[i];
		a[i] = a[n-1-i];
		a[n-1-i] = t;
	}
}

void
reverse(list_t **head, int k)
{
	if (*head == NULL || k < 2)
		return;

	int len = get_length(*head);
	if (k > len)
		return;

	uintptr_t *aux = (uintptr_t *)malloc(sizeof (uintptr_t) * len);
	if (aux == NULL) /* error: failed to malloc */
		return;

	/* copy addr of nodes to aux[] */
	int index = 0;
	for (list_t *p = *head; p != NULL; p = p->next)
		aux[index++] = (uintptr_t)p;

	/* reverse aux[] with gap k */
	for (int i = 0; i < len && len - i >= k; i += k)
		reverse_array(aux+i, k);

	/* rebuild list */
	((list_t *)aux[len-1])->next = NULL;
	for (int i = 0; i < len-1; i++)
		((list_t *)aux[i])->next = (list_t *)aux[i+1];
	*head = (list_t *)aux[0];

	free(aux);
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <k> <n1> [n2] ...\n", argv[0]);
		return -1;
	}

	int k = atoi(argv[1]);
	if (k < 1) {
		fprintf(stderr, "Oops, k=%d not supported\n", k);
		return 1;
	}

	argc -= 2;
	argv += 2;

	list_t *head = NULL;
	init(&head, argv, argc); /* create single linked list */

	show(head);
	reverse(&head, k);
	show(head);

	fini(head); /* destroy single linked list */

	return 0;
}
