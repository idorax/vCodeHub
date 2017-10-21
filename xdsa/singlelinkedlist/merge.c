/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * This small program is to merge two sorted single linked lists.
 *
 * Example:
 *        Input:  (a) 1->3->5->7->NULL
 *                (b) 2->4->6->8->NULL
 *        Output:     1->2->3->4->5->6->7->8->NULL
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct list_s {
	int data;
	struct list_s *next;
} list_t;

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
init(list_t **head, int *a, int n)
{
	list_t *new = NULL;
	list_t *cur = NULL;

	for (int i = 0; i < n; i++) {
		new = (list_t *)malloc(sizeof (list_t));
		if (new == NULL) /* error: failed to malloc */
			return;
		new->data = a[i];
		new->next = NULL;

		if (*head == NULL)
			*head = new;
		else
			cur->next = new;
		cur = new;
	}
}

/**
 * Insertion Sort on a Single Linked List : insert a node to the sorted list
 */
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
		if (p->data <= node->data) {
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

/**
 * Insertion Sort on a Single Linked List
 */
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

#ifdef _XXX_LIST_INSERT_

/**
 * Merge two sorted single linked lists (dst and src).
 */
list_t *
merge(list_t *head1, list_t *head2)
{
	if (head1 == NULL)
		return head2;

	if (head2 == NULL)
		return head1;

	/* now merge the two lists */
	list_t *out = NULL;
	list_t *p = NULL;
	if (head1->data < head2->data) {
		out = head1;
		p = head2;
	} else {
		out = head2;
		p = head1;
	}

	/*
	 * insert per node of list 'p' to the dst list one by one, and always
	 * pick up the previous node inserted as the new head for getting good
	 * time complexity once list_insert() is called
	 */
	list_t *head = out;
	while (p != NULL) {
		list_t *this = p;
		p = p->next;
		this->next = NULL;
		list_insert(&head, this);
		head = this;
	}

	return out;
}

#else /* with better performance */

static void
list_insert_node_tail(list_t **head, list_t *node)
{
	static list_t *tail = NULL;

	if (*head == NULL) {
		*head = tail = node;
		return;
	}

	tail->next = node;
	tail = node;
}

list_t *
merge(list_t *head1, list_t *head2)
{
	list_t *out = NULL;
	list_t *p1 = head1;
	list_t *p2 = head2;

	while (p1 != NULL && p2 != NULL) {
		list_t *node = NULL;

		if (p1->data < p2->data) {
			node = p1;		   /* 1. save p1 */
			p1 = p1->next;		   /* 2. move p1 forward */
		} else {
			node = p2;		   /* 1. save p2 */
			p2 = p2->next;		   /* 2. move p2 forward */
		}

		node->next = NULL;		   /* 3. cut node's next off */
		list_insert_node_tail(&out, node); /* 4. append node to out */
	}

	if (p1 != NULL) /* link the left of list 1 to the tail of out */
		list_insert_node_tail(&out, p1);

	if (p2 != NULL) /* link the left of list 2 to the tail of out */
		list_insert_node_tail(&out, p2);

	return out;
}

#endif

/**
 * Covert string (its delimiter is ',') to an array of int, which is similar to
 * strtok_r(). Note that the caller must free the memory of the array. e.g.
 *     char *s = "1,2,3,4,5,6"
 *     return {1, 2, 3, 4, 5, 6}
 */
static int *
str2list(char *s, int *len)
{
	int n = 0;

	/* get the number of elements in the string */
	char *p = s;
	while (*p != '\0') {
		if (*p == ',')
			n++;
		p++;
	}
	n++; /* Note the number of elements == the number of ',' + 1 */

	int *out = (int *)malloc(sizeof(int) * n);
	if (out == NULL) {
		*len = 0;
		return NULL;
	}
	*len = n;

	int j = 0;
	for (p = s; *p != '\0'; p++) {
		int k = 0;
		char buf[16] = { 0 }; /* max int is 2 ** 31 - 1 == 2147483647 */

		while (*p != ',' && *p != '\0') {
			buf[k++] = *p;
			p++;
		}

		*(out + j) = atoi(buf);
		j++;

		if (*p == '\0')
			break;
	}

	return out;
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <n1,n2,...> <m1,m2,...>\n", argv[0]);
		return -1;
	}

	int n = 0;
	int *l1 = str2list(argv[1], &n);
	int m = 0;
	int *l2 = str2list(argv[2], &m);

	list_t *head1 = NULL;
	list_t *head2 = NULL;
	init(&head1, l1, n); /* create linked list 1 */
	init(&head2, l2, m); /* create linked list 2 */

	free(l1); /* destroy l1 */
	free(l2); /* destroy l2 */

	list_sort(&head1); /* make sure linked list 1 is sorted */
	list_sort(&head2); /* make sure linked list 2 is sorted */

	show(head1);
	show(head2);
	list_t *p = merge(head1, head2);
	show(p);
	//list_t *p1 = merge(head1, NULL); show(p1);
	//list_t *p2 = merge(NULL, head2); show(p2);
	//list_t *p3 = merge(NULL, NULL);  show(p3);

	fini(head1); /* destroy linked list 1 */
	fini(head2); /* destroy linked list 2 */

	return 0;
}
