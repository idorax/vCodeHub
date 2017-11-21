/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * This small program is to detect a singly linked list has a loop or not.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static void
loop_init(list_t *head, int index, list_t **tail)
{
	if (head == NULL) {
		*tail = NULL;
		return;
	}

	/* get the tail node */
	list_t *p = head;
	while (p->next != NULL)
		p = p->next;
	*tail = p;

	/*
	 * find the node by index ([1..N]) to init the loop
	 *
	 * NOTE: If index == 0, don't init the loop
	 *       If index > length of the list, don't init the loop either
	 */
	int cnt = index;
	list_t *q = head;
	while (--cnt > 0 && q != NULL)
		q = q->next;

	/* now init the loop */
	if (cnt == 0)
		(*tail)->next = q;
}

static void
loop_fini(list_t *tail)
{
	if (tail != NULL)
		tail->next = NULL;
}

/**
 * Detect a singly linked list has a loop
 */
bool is_loop(list_t *head)
{
	list_t *fast = head;
	list_t *slow = head;

	/*
	 * If loop does not exist, fast should firstly reach the end
	 * a) if the length of list is even, fast       will be NULL
	 * b) if the length of list is odd,  fast->next will be NULL
	 */
	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;

		/*
		 * Well, loop is found as the fast catches up with the slow
		 */
		if (fast == slow)
			return true;
	}

	return false;
}

/**
 * Get the length of the loop if a singly linked list has a loop
 */
int get_loop_length(list_t *head)
{
	list_t *fast = head;
	list_t *slow = head;
	list_t *node = NULL;

	/* get a node in the loop */
	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;

		if (fast == slow) {
			node = slow;
			break;
		}
	}

	/* no loop found hence the length should be zero */
	if (node == NULL)
		return 0;

	/* now walk again to get the length of the loop */
	int len = 1;
	for (list_t *p = node->next; p != node; p = p->next)
		len++;
	return len;
}

/**
 * Get the joint if a singly linked list has a loop
 */
list_t *
get_loop_joint(list_t *head)
{
	list_t *fast = head;
	list_t *slow = head;
	list_t *node = NULL;

	/* get a node in the loop */
	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;

		if (fast == slow) {
			node = slow;
			break;
		}
	}

	/* no loop found hence the joint should be NULL */
	if (node == NULL)
		return NULL;

	/*
	 * The slow walks the loop from the node, and let the fast walk the
	 * list from its head. They should meet at the joint.
	 *
	 *     Head        Joint
	 *     |           |
	 *     O-->O-->O-->O<--O<--O
	 *                 |       ^
	 *                 V       |
	 *                 O-->O-->O
	 *                          \
	 *                           Node
	 *
	 *     x : The length from Head  to Joint
	 *     y1: The length from Joint to Node
	 *     y2: The length from Node  to Joint
	 *
	 *     Total steps of the slow walked: x + y1
	 *     Total steps of the fast walked: x + y1 + y2 + y1
	 *     Note the fast and the slow have the same times to move,
	 *     So                              x + y1 == (x + y1 + y2 + y1) / 2
	 *                                 ==> 2x + 2y1 == x + 2y1 + y2
	 *                                 ==> x == y2
	 */
	fast = head;
	slow = node;
	while (fast != slow) {
		fast = fast->next;
		slow = slow->next;
	}

	return slow;
}

/**
 * Get the total length of a singly linked list even though it has a loop
 */
int get_total_length(list_t *head)
{
	int len = get_loop_length(head);

	list_t *joint =	(len != 0) ? get_loop_joint(head) : NULL;

	for (list_t *p = head; p != joint; p = p->next)
		len++;

	return len;
}

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
		fprintf(stderr, "Usage: %s <num> <n1,n2,...>\n",
			argv[0]);
		return -1;
	}

	int idx = atoi(argv[1]);

	int n = 0;
	int *l = str2list(argv[2], &n);

	list_t *head = NULL;
	init(&head, l, n); /* create linked list */

	free(l); /* destroy l */

	show(head);

	/* create a loop */
	list_t *tail = NULL;
	loop_init(head, idx, &tail);

	if (is_loop(head)) {
		int len = get_loop_length(head);
		list_t *joint = get_loop_joint(head);
		printf("LOOP FOUND\n");
		printf("The length of loop is %d\n", len);
		printf("The first joint    is %d (%p)\n", joint->data, joint);
	} else {
		fprintf(stderr, "LOOP NOT FOUND\n");
	}
	printf("The total length   is %d\n", get_total_length(head));

	/* destroy the loop */
	loop_fini(tail);

	fini(head);

	return 0;
}
