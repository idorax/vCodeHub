/*
 * Copyright (C) 2017, Vector Li (idorax@126.com)
 */

#ifndef _LIBSTACK_H
#define _LIBSTACK_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef void * uintptr_t; /* generic pointer to any struct */

uintptr_t *stack_init(size_t size);
void stack_fini();
int stack_isFull();
int stack_isEmpty();
void push(uintptr_t e);
void pop(uintptr_t *e);

#ifdef	__cplusplus
}
#endif

#endif /* _LIBSTACK_H */
