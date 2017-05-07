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
