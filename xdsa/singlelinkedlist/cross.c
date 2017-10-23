/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * This small program is to get the first node of two single linked lists
 * if they converge. If not found, return NULL.
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

static list_t *
crpt_init(list_t *head1, list_t *head2, int index)
{
	list_t *xnode = NULL;

	if (head1 == NULL)
		return head2;

	if (head2 == NULL)
		return NULL;

	/* get the xnode of list head1 */
	int i = 0;
	for (list_t *p = head1; p != NULL; p = p->next) {
		i++;
		if (i == index) {
			xnode = p;
			break;
		}
	}

	/* append the xnode to list head2 */
	list_t *p = head2;
	while (p->next != NULL)
		p = p->next;
	p->next = xnode;

	return head2;
}

/**
 * Reverse a single linked list
 */
static void
reverse(list_t **head)
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
		list_t *next = this->next;
		this->next = prev;
		prev = this;
		this = next;
	}

	*head = newhead;
}

/**
 * Detect list 1 and 2 have crosspoint, return true if yes
 */
static bool
crpt_detect(list_t *head1, list_t *head2)
{
	if (head1 == NULL)
		return false;

	if (head2 == NULL)
		return false;

	bool flag = false;

	/* 0. save the original head of list 2 */
	list_t *head2orig = head2;

	/* 1. reverse list 2 */
	reverse(&head2);

	/* 2. walk list 1, return true if original head of list 2 is reached */
	for (list_t *p = head1; p != NULL; p = p->next) {
		if (p == head2orig) {
			flag = true;
			break;
		}
	}

	/* 3. reverse list 2 back */
	reverse(&head2);

	return flag;
}

/**
 * Get the first node address of crosspoint of list 1 and 2
 */
list_t *
crpt_get(list_t *head1, list_t *head2)
{
	list_t *xnode = NULL;

	bool flag = crpt_detect(head1, head2);
	if (!flag)
		return NULL;

	for (list_t *p = head1; p != NULL; p = p->next) {
		for (list_t *q = head2; q != NULL; q = q->next) {
			if (q == p) {
				xnode = p;
				goto done;
			}
		}
	}

done:
	return xnode;
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
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <num> <n1,n2,...> <m1,m2,...>\n",
			argv[0]);
		return -1;
	}

	int idx = atoi(argv[1]);

	int n = 0;
	int *l1 = str2list(argv[2], &n);
	int m = 0;
	int *l2 = str2list(argv[3], &m);

	list_t *head1 = NULL;
	list_t *head2 = NULL;
	init(&head1, l1, n); /* create linked list 1 */
	init(&head2, l2, m); /* create linked list 2 */

	free(l1); /* destroy l1 */
	free(l2); /* destroy l2 */

	show(head1);
	show(head2);

	list_t *head3 = crpt_init(head1, head2, idx);
	show(head3);

	list_t *p = crpt_get(head1, head3);
	if (p == NULL)
		printf("Crosspoint not found\n");
	else
		show(p);

	fini(head1);
	fini(head2);

	return 0;
}
