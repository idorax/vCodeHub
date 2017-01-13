/*
 * Copyright (C) 2017, Vector Li (idorax@126.com)
 */

/**
 * foo1.c : Test case to verify bst_del_node() works fine.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../libbst.h"

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <key>\n", argv[0]);
		return -1;
	}

	key_t key = atoi(argv[1]);

	get_debug(); /* Note ENV is DEBUG */

	bst_node_t *root = NULL;

	key_t a[] = {10, 30, 50, 20, 40, 60, 80, 70, 90, 100, 120, 110, 15};
	size_t n = sizeof(a) / sizeof(key_t);

	bst_init(&root, a, n);

	if (key == -1) { /* delete all */
		bst_walk(root, WALK_ORDER_IN); printf("\n");

		int i;
		for (i = 0; i < n; i++) {
			bst_del_node(&root, a[i]);
			bst_walk(root, WALK_ORDER_IN); printf("\n");
		}
	} else {
		printf("To delete node whose key=%d\n", key);
		bst_walk(root, WALK_ORDER_IN); printf("\n");
		bst_del_node(&root, key);
		bst_walk(root, WALK_ORDER_IN); printf("\n");
	}

	bst_fini(root);

	return (0);
}
