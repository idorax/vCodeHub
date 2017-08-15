/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#ifndef _LIBBIGNUM_H
#define _LIBBIGNUM_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef unsigned char		byte;	/* 1 byte */
typedef unsigned short		word;	/* 2 bytes */
typedef unsigned int		dword;	/* 4 bytes */
typedef unsigned long long	qword;	/* 8 bytes */
typedef signed char		sbyte;	/* 1 byte */
typedef signed short		sword;	/* 2 bytes */
typedef signed int		sdword;	/* 4 bytes */
typedef signed long long	sqword;	/* 8 bytes */

typedef struct big_number_s {
	dword *data;
	dword size;
} big_number_t;

typedef enum {false, true} bool;

big_number_t *big_number_add(big_number_t *a, big_number_t *b);
big_number_t *big_number_sub(big_number_t *a, big_number_t *b);
big_number_t *big_number_mul(big_number_t *a, big_number_t *b);
void free_big_number(big_number_t *p);
void dump_big_number(char *tag, big_number_t *p);

big_number_t *str2bn(const char *s);
char *bn2str(big_number_t *p);

bool gt(big_number_t *a, big_number_t *b);
bool lt(big_number_t *a, big_number_t *b);
bool eq(big_number_t *a, big_number_t *b);
#define ge(a, b) !lt((a), (b))
#define le(a, b) !gt((a), (b))
#define ne(a, b) !eq((a), (b))

#ifdef	__cplusplus
}
#endif

#endif /* _LIBBIGNUM_H */
