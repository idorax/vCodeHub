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

typedef struct big_number_s {
	dword *data;
	dword size;
} big_number_t;

big_number_t *big_number_mul(big_number_t *a, big_number_t *b);
void free_big_number(big_number_t *p);

big_number_t *str2bn(const char *s);
char *bn2str(big_number_t *p);

#ifdef	__cplusplus
}
#endif

#endif /* _LIBBIGNUM_H */
