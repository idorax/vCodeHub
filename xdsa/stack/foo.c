/*
 * Copyright (C) 2017, Vector Li (idorax@126.com)
 */

/**
 * A simple test against stack OPs, including:
 * o stack_init(),   stack_fini()
 * o stack_isFull(), stack_isEmpty()
 * o push(), pop()
 */

#include <stdio.h>
#include "libstack.h"

static void
dump_stack(uintptr_t *ss, size_t size)
{
	(void) printf("%p: ", ss);
	for (int i = 0; i < size; i++) {
		if (ss[i] != NULL)
			(void) printf("%-10p ", *(ss+i));
		else
			(void) printf("0x%-8x ", 0x0);
	}
	printf("\n");
}

int
main(int argc, char *argv[])
{
	size_t size = 4;

	uintptr_t *ss = stack_init(size);
	dump_stack(ss, size);

	for (int i = 0; !stack_isFull(); i++) {
		push((uintptr_t)(ss+i));
		dump_stack(ss, size);
	}

	(void) printf("\n");

	uintptr_t e = NULL;
	for (; !stack_isEmpty();) {
		pop(&e);
		(void) printf(" (pop) got %-10p\n", e);
	}

	stack_fini();

	return 0;
}
