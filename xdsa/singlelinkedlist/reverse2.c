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

#ifdef _USE_AUX
static void
reverse_single_linked_list(list_t **head)
{
	if (*head == NULL)
		return;

	int len = get_length(*head);
	if (len < 2)
		return;

	list_t **aux = (list_t **)malloc(sizeof (list_t *) * len);
	if (aux == NULL) /* error */
		return;

	/* save addr of per node to aux[] */
	int index = 0;
	for (list_t *p = *head; p != NULL; p = p->next) {
		aux[len-1-index] = p;
		index++;
	}

	/* rebuild the linked list by walking aux[] */
	*head = aux[0];
	for (int i = 0; i < len-1; i++)
		aux[i]->next = aux[i+1];
	aux[len-1]->next = NULL;

	free(aux);
}
#else
static void
reverse_single_linked_list(list_t **head)
{
	list_t *newhead = NULL;
	list_t *this = *head;
	list_t *prev = NULL;

	while (this != NULL) {
		/*
		 * If this->next is NULL, this is the tail node, which should
		 * be the new head
		 */
		if (this->next == NULL)
			newhead = this;

		/*
		 * ListIn:  prevNode -> thisNode -> nextNode
		 * ListOut: prevNode <- thisNode <- nextNode
		 *          1. thisNode->next = prevNode;
		 *          2.       prevNode = thisNode;
		 *          3.       thisNode = thisNode->next;
		 */
		list_t *t = this->next;
		this->next = prev;
		prev = this;
		this = t;
	}

	*head = newhead;
}
#endif

/*
 * join single linked list L2 to L1
 */
static void
join(list_t *l1, list_t *l2)
{
	list_t *tail = NULL;
	for (list_t *p = l1; p != NULL; p = p->next)
		tail = p;

	if (tail != NULL)
		tail->next = l2;
}

/*
 * cut single linked list at Kth position, K = 0, 1, ...
 */
static list_t *
cut(list_t *head, int k)
{
	if (head == NULL)
		return NULL;

	list_t *p = head;
	for (int i = 0; i < k; i++)
		p = p->next;

	list_t *q = p->next;
	p->next = NULL;
	return q;
}

static void
reverse(list_t **head, int k)
{
	if (*head == NULL || k < 2)
		return;

	int len = get_length(*head);
	if (k > len)
		return;

	int aux_sz = len / k + 1;
	list_t **aux = (list_t **)malloc(sizeof (list_t *) * aux_sz);
	if (aux == NULL) /* error */
		return;

	/* reverse with gap k */
	int index = 0;
	list_t *headp = *head;
	for (int i = 0; i < len; i += k) {
		if (len - i < k) {
			aux[index] = headp;
			break;
		}

		list_t *nexthead = cut(headp, k-1);
		reverse_single_linked_list(&headp);
		aux[index++] = headp;

		headp = nexthead;
	}

	/* join per list in aux[] */
	headp = aux[0];
	for (int i = 1; i < aux_sz; i++)
		join(headp, aux[i]);

	*head = headp;
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
	init(&head, argv, argc); /* create linked list */

	show(head);
	reverse(&head, k);
	show(head);

	fini(head); /* destroy linked list */

	return 0;
}
