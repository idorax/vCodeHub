/*
 * Copyright (C) 2017, Vector Li (idorax@126.com)
 */
#include <stdio.h>
#include <stdlib.h>
#include "libstack.h"

/**
 * Basic Stack OPs are supported, including:
 *
 * 1. Construct/Destruct a stack
 * 2. Tell stack is full or empty
 * 3. push() and pop()
 *
 * == DESIGN NOTES ==
 *
 * There are 3 static variables reserved,
 *
 *     ss: stack segment
 *     sp: stack pointer
 *     sz: stack size
 *
 * And the stack looks like:
 *
 *               | RED | ss[-1]  ; SHOULD NEVER BE ACCESSED
 *     low-addr  +-----+ <------------TopOfStack-----------
 *        ^      |     | ss[0]
 *        |      |     | ss[1]
 *        |      | ... |
 *        |      |     |
 *        |      |     | ss[sz-1]
 *        |      +-----+ <------------BottomOfStack--------
 *     high-addr | RED | ss[sz]  ; SHOULD NEVER BE ACCESSED
 *
 * (1) If (sp - ss) == 0,  stack is full
 * (2) If (sp - ss) == sz, stack is empty
 * (3) Push(E): { sp -= 1;   *sp = E; }
 * (4) Pop(&E): { *E  = *sp; sp += 1; }
 */

static uintptr_t *ss = NULL; /* stack segment */
static uintptr_t *sp = NULL; /* stack pointer */
static size_t sz = 0;        /* stack size */

int stack_isFull()  { return (sp == ss); }
int stack_isEmpty() { return (sp == ss + sz); }

uintptr_t *
stack_init(size_t size)
{
	ss = (uintptr_t *)malloc(sizeof (uintptr_t) * size);
	if (ss == NULL) {
		fprintf(stderr, "failed to malloc\n");
		return NULL;
	}

	sz = size;
	sp = ss + size;
	return ss;
}

void
stack_fini()
{
	free(ss);
}

void
push(uintptr_t e)
{
	sp -= 1;
	*sp = e;
}

void
pop(uintptr_t *e)
{
	*e = *sp;
	sp += 1;
}
