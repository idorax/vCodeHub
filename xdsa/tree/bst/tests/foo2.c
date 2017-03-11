/*
 * Copyright (C) 2017, Vector Li (idorax@126.com)
 */

/**
 * foo1.c : Test case to verify bst_get_depth() works fine.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../libbst.h"

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <key> [...]\n", argv[0]);
		return -1;
	}

	argc--;
	argv++;

	size_t n = argc;
	key_t *a = (key_t *)malloc(sizeof(key_t) * n);
	if (a == NULL) {
		fprintf(stderr, "failed to malloc()\n");
		return -1;
	}
	for (int i = 0; i < n; i++)
		*(a+i) = atoi(argv[i]);

	get_debug(); /* Note ENV is DEBUG */

	bst_node_t *root = NULL;

	bst_init(&root, a, n);

	bst_walk(root, WALK_ORDER_IN); printf("\n");

	int depth = bst_get_depth(root);
	printf("The depth of BST is %d\n", depth);

	bst_fini(root);

	free(a);

	return (0);
}
