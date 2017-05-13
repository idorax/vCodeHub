/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#ifndef _LIST_H
#define _LIST_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * offsetof - offset of a structure member
 * @TYPE:       the type of the struct.
 * @MEMBER:     the name of the member within the struct.
 */
#define offsetof(TYPE, MEMBER) ((size_t)(&(((TYPE *)0)->MEMBER)))

typedef struct list_s {
	struct list_s *next;
	size_t offset;
} list_t;

list_t *list_d2l(void *object, size_t offset);
void   *list_l2d(list_t *list);

#define LIST_INIT_NODE(list, offset) do {	\
		(list)->next = NULL;		\
		(list)->offset = (offset);	\
	} while (0)

void list_insert_tail(list_t **head, void *object, size_t offset);
void list_insert_head(list_t **head, void *object, size_t offset);

void list_insert_node_tail(list_t **head, list_t *node);
void list_insert_node_head(list_t **head, list_t *node);

void list_insert_node_after(list_t **head, list_t *node1, list_t *node2);
void list_insert_node_before(list_t **head, list_t *node1, list_t *node2);

void list_delete_node(list_t **head, list_t *node);

#ifdef	__cplusplus
}
#endif

#endif /* _LIST_H */
