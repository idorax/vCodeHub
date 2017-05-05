/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct foo_s {
	int	data;
	list_t	link;
} foo_t;

static void
foo_init(list_t **head, void *object, size_t offset)
{
	if (object == NULL)
		return;

	printf("init (node) %p\n", object);
	list_t *node = list_d2l(object, offset);

	if (*head == NULL) {
		*head = node;
		return;
	}

	list_t *tail = NULL;
	for (list_t *p = *head; p != NULL; p = p->next)
		tail = p;
	tail->next = node;
}

static void
foo_fini(list_t *head)
{
	list_t *p = head;
	while (p != NULL) {
		list_t *q = p;
		p = p->next;

		void *obj = list_l2d(q);
		printf("free (node) %p next (list) %p\n", obj, p);
		free(obj);
	}
}

static void
foo_show(list_t *head)
{
	for (list_t *p = head; p != NULL; p = p->next) {
		foo_t *obj = list_l2d(p);

		printf("show (list) %p next (list) %p \t: "
		    "show (node) %p = {0x%x, {%p, %d}}\n",
		    &obj->link, obj->link.next,
		    obj, obj->data, obj->link.next, obj->link.offset);
	}
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <num>\n", argv[0]);
		return -1;
	}

	list_t *head = NULL;
	for (int i = 0; i < atoi(argv[1]); i++) {
		foo_t *p = (foo_t *)malloc(sizeof (foo_t));
		if (p == NULL) /* error */
			return -1;
		p->data = 0x1001 + i;

		foo_init(&head, (void *)p, offsetof(foo_t, link));
	}

	foo_show(head);
	foo_fini(head);

	return 0;
}
