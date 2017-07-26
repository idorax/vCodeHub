/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbignum.h"

/*
 * Add 64-bit number (8 bytes) to a[] whose element is 32-bit int (4 bytes)
 *
 * e.g.
 *      a[] = {0x12345678,0x87654321,0x0}; n = 3;
 *      n64 =  0xffffffff12345678
 *
 *      The whole process of add64() looks like:
 *
 *             0x12345678 0x87654321 0x00000000
 *          +  0x12345678 0xffffffff
 *          -----------------------------------
 *          =  0x2468acf0 0x87654321 0x00000000
 *          +             0xffffffff
 *          -----------------------------------
 *          =  0x2468acf0 0x87654320 0x00000001
 *
 *      Finally,
 *      a[] = {0x2468acf0,0x87654320,0x00000001}
 */
static void
add64(dword a[], dword n, qword n64)
{
	dword carry = 0;

	carry = n64 & 0xFFFFFFFF; /* low 32 bits of n64 */
	for (dword i = 0; i < n; i++) {
		if (carry == 0x0)
			break;

		qword t = (qword)a[i] + (qword)carry;
		a[i] = t & 0xFFFFFFFF;
		carry = (dword)(t >> 32); /* next carry */
	}

	carry = (dword)(n64 >> 32); /* high 32 bits of n64 */
	for (dword i = 1; i < n; i++) {
		if (carry == 0x0)
			break;

		qword t = (qword)a[i] + (qword)carry;
		a[i] = t & 0xFFFFFFFF;
		carry = (dword)(t >> 32); /* next carry */
	}
}

big_number_t *
big_number_mul(big_number_t *a, big_number_t *b)
{
	big_number_t *c = (big_number_t *)malloc(sizeof(big_number_t));
	if (c == NULL) /* malloc error */
		return NULL;

	c->size = a->size + b->size;
	c->data = (dword *)malloc(sizeof(dword) * c->size);
	if (c->data == NULL) /* malloc error */
		return NULL;

	memset(c->data, 0, sizeof(dword) * c->size);

	dword *adp = a->data;
	dword *bdp = b->data;
	dword *cdp = c->data;
	for (dword i = 0; i < a->size; i++) {
		if (adp[i] == 0x0)
			continue;

		for (dword j = 0; j < b->size; j++) {
			if (bdp[j] == 0x0)
				continue;

			qword n64 = (qword)adp[i] * (qword)bdp[j];
			dword *dst = cdp + i + j;
			add64(dst, c->size - (i + j), n64);
		}
	}

	return c;
}

void
free_big_number(big_number_t *p)
{
	if (p == NULL)
		return;

	if (p->data != NULL)
		free(p->data);

	free(p);
}

static char *
str2bn_align(char *s)
{
	size_t n = strlen(s);
	int m = (n % 8 != 0 ) ? (8 - (n % 8)) : 0;
	char *p = (char *)malloc(sizeof(char) * (n + m + 1));
	if (p == NULL)
		return NULL;

	for (int i = 0; i < m; i++)
		*(p + i) = '0';

	snprintf(p + m, n + m + 1, "%s", s);
	*(p + n + m) = '\0';

	return p;
}

/**
 * convert string to big number
 *
 * Example:
 *	string: 0x12345678012345679abcdef0
 *	bignum: p->data : {0x9abcdef0, 0x01234567, 0x12345678, 0x00000000}
 *	        p->size : 0x4
 *
 * More Examples:
 *	1. 0x1234567           : 0x01234567, 0x00000000
 *	2. 0x12345678          : 0x12345678, 0x00000000
 *	3. 0x123456789         : 0x23456789, 0x00000001, 0x00000000
 *	4. 0x12345678abcdefab  : 0xabcdefab, 0x12345678, 0x00000000
 *	5. 0x12345678abcdefabc : 0xbcdefabc, 0x2345678a, 0x00000001, 0x00000000
 */
big_number_t *
str2bn(const char *s)
{
	if (s == NULL || *s == '\0')
		return NULL;

	size_t n = strlen(s);
	char *p = (char *)s;
	if (*p == '0' && *(p + 1) == 'x') {
		p += 2;
		n -= 2;
	}

	big_number_t *c = (big_number_t *)malloc(sizeof(big_number_t));
	if (c == NULL) /* malloc error */
		return NULL;

	c->size = (n % 8 == 0) ? (n / 8 + 1) : (n / 8 + 2);
	c->data = (dword *)malloc(sizeof(dword) * c->size);
	if (c->data == NULL) /* malloc error */
		return NULL;

	memset(c->data, 0, sizeof(dword) * c->size);

	char *q = str2bn_align(p);
	if (q == NULL)
		return NULL;
	n = strlen(q);

	for (size_t i = 0; i < n / 8; i++) {
		char buf[9] = { 0 };
		strncpy(buf, q + i * 8, 8);
		size_t id = n / 8 - 1 - i;
		sscanf(buf, "%x", (unsigned int *)(c->data + id));
	}

	free(q); q = NULL;

	return c;
}

/**
 * convert big number to string
 */
char *
bn2str(big_number_t *bn)
{
	size_t n = bn->size * 8;
	char *p = (char *)malloc(sizeof(char) * (n + 3));
	if (p == NULL)
		return NULL;

	memset(p, 0, n + 3);
	*(p + 0) = '0';
	*(p + 1) = 'x';

	size_t m = bn->size;
	for (int i = bn->size - 1; i >= 0; i--) {
		if ((bn->data[i]) != 0x0)
			break;
		m--;
	}

	for (int i = m - 1; i >= 0; i--) {
		char buf[9] = { 0 };
		snprintf(buf, sizeof(buf), "%08x", (bn->data)[i]);
		strncat(p, buf, n - 2);
	}

	return p;
}
