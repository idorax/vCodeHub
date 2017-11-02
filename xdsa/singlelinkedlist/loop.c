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

static void
loop_init(list_t *head, int index)
{
	/* XXX: TBD */
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

	/* XXX: create a loop */
	loop_init(head, idx);

	fini(head);

	return 0;
}
