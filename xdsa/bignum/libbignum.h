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

/**
 * Sign of a big number
 * + : 0, positive
 * - : 1, negative
 */
typedef enum sign_s {sign_pos=0, sign_neg} sign_t;

/**
 * A big number is defined as three parts in the following,
 *       1. sign: 0 - positive, 1 - negative
 *       2. data: a block of continuous dword (unsigned int)
 *       3. size: the length of data[]
 *
 * NOTE:
 *       1. In data[], data[] must end with 0x0. That is, the size of a big
 *          number should be always >= 1.
 *       2. We often reset the size of big number because it may end with
 *          more than one 0x0. Hence, size may less than
 *          sizeof(data)/sizeof(dword).
 */
typedef struct big_number_s {
	sign_t sign;
	dword *data;
	dword size;
} big_number_t;

typedef enum {false, true} bool;

big_number_t *bn_add(big_number_t *a, big_number_t *b);
big_number_t *bn_sub(big_number_t *a, big_number_t *b);
big_number_t *bn_mul(big_number_t *a, big_number_t *b);
big_number_t *bn_fac(big_number_t *a);

void free_big_number(big_number_t *p);
void dump_big_number(char *tag, big_number_t *p);
#define dump(tag, p) dump_big_number(tag, p)

big_number_t *str2bn(const char *s);
char *bn2str(big_number_t *p);

bool abs_gt(big_number_t *a, big_number_t *b);
bool abs_lt(big_number_t *a, big_number_t *b);
bool abs_eq(big_number_t *a, big_number_t *b);
#define abs_ge(a, b) !abs_lt((a), (b))
#define abs_le(a, b) !abs_gt((a), (b))
#define abs_ne(a, b) !abs_eq((a), (b))

#ifdef	__cplusplus
}
#endif

#endif /* _LIBBIGNUM_H */
