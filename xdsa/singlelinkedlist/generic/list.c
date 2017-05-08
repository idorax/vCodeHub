/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Simple generic single linked list implementation
 */

#include <stdio.h>
#include "list.h"

/*
 * Cast ptr of DATA object node to ptr of LIST node
 */
list_t *
list_d2l(void *object, size_t offset)
{
	if (object == NULL)
		return NULL;

	return (list_t *)((char *)object + offset);
}

/*
 * Cast ptr of LIST node to ptr of DATA object node
 */
void *
list_l2d(list_t *list)
{
	if (list == NULL)
		return NULL;

	return (void *)((char *)list - list->offset);
}

/*
 * Insert an object after the tail of list
 */
void
list_insert_tail(list_t **head, void *object, size_t offset)
{
	if (object == NULL)
		return;

	list_t *node = list_d2l(object, offset);
	LIST_INIT_NODE(node, offset);

	if (*head == NULL) {
		*head = node;
		return;
	}

	list_t *tail = NULL;
	for (list_t *p = *head; p != NULL; p = p->next)
		tail = p;
	tail->next = node;
}

/*
 * Insert an object before the head of list
 */
void
list_insert_head(list_t **head, void *object, size_t offset)
{
	if (object == NULL)
		return;

	list_t *node = list_d2l(object, offset);
	LIST_INIT_NODE(node, offset);

	if (*head == NULL) {
		*head = node;
		return;
	}

	node->next = *head;
	*head = node;
}

/*
 * Delete a node from list
 */
void
list_delete(list_t **head, list_t *node)
{
	if (head == NULL || *head == NULL || node == NULL)
		return;

	if (*head == node) {
		*head = node->next;
		return;
	}

	list_t *q = *head;
	for (list_t *p = *head; p != NULL; p = p->next) {
		if (p == node)
			break;
		q = p;
	}
	q->next = node->next;
}
