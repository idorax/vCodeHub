/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "list.h"

typedef struct foo_s {
	int	data;
	list_t	link;
} foo_t;

typedef enum {false, true} bool_t;
bool_t g_reverse = false;

static void
foo_init(list_t **head, int n)
{
	for (int i = 0; i < n; i++) {
		foo_t *p = (foo_t *)malloc(sizeof (foo_t));
		if (p == NULL) /* error */
			return;
		p->data = 0x1001 + i;

		printf("init (node) %p {%#x}\n", p, p->data);
		size_t offset = offsetof(foo_t, link);
		if (g_reverse)
			list_insert_head(head, (void *)p, offset);
		else
			list_insert_tail(head, (void *)p, offset);
	}
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

static void
foo_delete(list_t **head, int num)
{
	list_t *node = NULL;
	for (list_t *p = *head; p != NULL; p = p->next) {
		foo_t *obj = list_l2d(p);
		if (obj->data == num) {
			node = p;
			break;
		}
	}
	if (node == NULL)
		return;

	printf("\ndel  (list) %p next (list) %p ", node, node->next);
	list_delete_node(head, node);

	foo_t *p = list_l2d(node);
	printf("\t: free (node) %p = {%#x, {%p, %d}}\n\n",
	    p, p->data, node->next, node->offset);
	free(p);

	foo_show(*head);
}

static list_t *
foo_find(list_t *head, int data)
{
	for (list_t *p = head; p != NULL; p = p->next) {
		foo_t *obj = list_l2d(p);
		if (obj->data == data)
			return p;
	}
	return NULL;
}

static list_t *
foo_new(int data)
{
	foo_t *obj = (foo_t *)malloc(sizeof (foo_t));
	if (obj == NULL) /* error */
		return NULL;
	obj->data = data;

	size_t offset = offsetof(foo_t, link);
	list_t *node = list_d2l(obj, offset);
	LIST_INIT_NODE(node, offset);

	return node;
}

static void
foo_insert_before(list_t **head, int n1, int n2)
{
	list_t *node2 = foo_find(*head, n2);
	list_t *node1 = foo_new(n1);

	printf("\ninst (list) %p before(list)%p\n\n", node1, node2);
	list_insert_node_before(head, node1, node2);
	foo_show(*head);
}

static void
foo_insert_after(list_t **head, int n1, int n2)
{
	list_t *node2 = foo_find(*head, n2);
	list_t *node1 = foo_new(n1);

	printf("\ninst (list) %p after (list)%p\n\n", node1, node2);
	list_insert_node_after(head, node1, node2);
	foo_show(*head);
}

static void
usage(char *s)
{
	fprintf(stderr, "Usage: %s [-d <element>] [-R]"
	        " [<-i|-I> <new element>] <num>\n", s);
	fprintf(stderr, "  e.g. %s 10\n", s);
	fprintf(stderr, "       %s -R 10\n", s);
	fprintf(stderr, "       %s -d 1001 10\n", s);
	fprintf(stderr, "       %s -i 9001 -d 1001 10\n", s);
	fprintf(stderr, "       %s -I 9001 -d 1001 10\n", s);
}

int
main(int argc, char *argv[])
{
	char *arg0 = argv[0];

	int num2delete = -1;
	int num2insert = -1;
	int num2insert2 = -1;
	char c;
	while ((c = getopt(argc, argv, ":d:i:I:Rh")) != -1) {
		switch(c) {
		case 'd': /* delete */
			sscanf(optarg, "%x", &num2delete);
			break;

		case 'i': /* insert a before b */
			sscanf(optarg, "%x", &num2insert);
			break;

		case 'I': /* insert a after b */
			sscanf(optarg, "%x", &num2insert2);
			break;

		case 'R': /* reverse */
			g_reverse = true;
			break;

		case 'h': /* help */
			usage(arg0);
			return -1;

		case ':':
			fprintf(stderr, "Option '-%c' wants an argument\n",
			        optopt);
			return -1;

		case '?':
			fprintf(stderr, "%s: -%c invalid option\n",
			        arg0, optopt);
			return -1;

		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage(arg0);
		return -1;
	}

	list_t *head = NULL;
	foo_init(&head, atoi(argv[0]));
	foo_show(head);
	if (num2insert != -1)
		foo_insert_before(&head, num2insert, num2delete);
	if (num2insert2 != -1)
		foo_insert_after(&head, num2insert2, num2delete);
	if (num2delete != -1)
		foo_delete(&head, num2delete);
	foo_fini(head);

	return 0;
}
