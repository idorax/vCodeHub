/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbignum.h"

/**
 * Dump big number with specified tag
 */
void
dump_big_number(char *tag, big_number_t *p)
{
	if (p == NULL)
		return;

	printf("%s : sign=%d : data=%p : size=%d :\t",
	       tag, p->sign, p, p->size);
	for (dword i = 0; i < p->size; i++)
		printf("0x%08x ", p->data[i]);
	printf("\n");
}

/**
 * New a big number with size and initial value (dword)
 */
static big_number_t *
new_big_number(dword size, dword iv)
{
	big_number_t *p = (big_number_t *)malloc(sizeof(big_number_t));
	if (p == NULL) /* malloc error */
		return NULL;

	p->size = size;
	p->data = (dword *)malloc(sizeof(dword) * p->size);
	if (p->data == NULL) { /* malloc error */
		free(p);
		return NULL;
	}

	memset(p->data, 0, sizeof(dword) * p->size);
	p->data[0] = iv;

	return p;
}

/**
 * Get valid size of a big number in case it ends with more than two 0x0.
 * Note every big number must end with at least 1. that is, if the value
 * of a big number is 0x0, its size should be 1.
 *
 * e.g.
 *     1. bn->data = {0x12345678, 0x0, 0x0, 0x0}
 *        bn->size = 0x4
 *
 *        The valid size is 0x2 as a matter of fact
 *
 *     2. bn->data = {0x0, 0x0, 0x0}
 *        bn->size = 0x3
 *        The valid size is 0x1 as a matter of fact
 */
static dword
bn_get_valid_size(big_number_t *p)
{
	if (p == NULL)
		return 0;

	if (p->data == NULL)
		return 0;

	dword n = p->size;
	for (sqword i = (sqword)p->size - 1; i >= 0; i--) {
		if (*(p->data + i) != 0x0)
			break;
		n--;
	}

	return (n + 1);
}

/**
 * Compare big number a with b, return true if a is greater than b.
 */
bool
abs_gt(big_number_t *a, big_number_t *b)
{
	dword sa = bn_get_valid_size(a);
	dword sb = bn_get_valid_size(b);

	if (sa > sb)
		return true;
	else if (sa < sb)
		return false;
	else { /* sa == sb */
		for (sqword i = (sqword)sa - 1; i >= 0; i--) {
			if (a->data[i] > b->data[i])
				return true;
			else if (a->data[i] < b->data[i])
				return false;
			else /* (a->data[i] == b->data[i]) */
				continue;
		}
		return false; /* a->data[@] == b->data[@] */
	}
}

/**
 * Compare big number a with b, return true if a is less than b.
 */
bool
abs_lt(big_number_t *a, big_number_t *b)
{
	dword sa = bn_get_valid_size(a);
	dword sb = bn_get_valid_size(b);

	if (sa < sb)
		return true;
	else if (sa > sb)
		return false;
	else { /* sa == sb */
		for (sqword i = (sqword)sa - 1; i >= 0; i--) {
			if (a->data[i] < b->data[i])
				return true;
			else if (a->data[i] > b->data[i])
				return false;
			else /* (a->data[i] == b->data[i]) */
				continue;
		}
		return false; /* a->data[@] == b->data[@] */
	}
}

/**
 * Compare big number a with b, return true if a is equal to b.
 */
bool
abs_eq(big_number_t *a, big_number_t *b)
{
	dword sa = bn_get_valid_size(a);
	dword sb = bn_get_valid_size(b);

	if (sa != sb)
		return false;
	else { /* sa == sb */
		for (sqword i = (sqword)sa - 1; i >= 0; i--) {
			if (a->data[i] != b->data[i])
				return false;
			else /* (a->data[i] == b->data[i]) */
				continue;
		}
		return true; /* a->data[@] == b->data[@] */
	}
}

/**
 * Verify a big number is zero or not.
 */
static bool
is_zero(big_number_t *p)
{
	if (p == NULL)
		return false;

	if (p->data == NULL)
		return false;

	dword counter = 0;
	for (dword i = 0; i < p->size; i++) {
		if (p->data[i] == 0x0)
			counter++;
	}

	return (counter == p->size ? true : false);
}

/**
 * Add 64-bit number (8 bytes) to a[] whose element is 32-bit int (4 bytes)
 *
 * e.g.
 *      a[] = {0x12345678,0x87654321,0x0}; n = 3;
 *      n64 =  0xffffffff12345678
 *
 *      The whole process of abs_add64() looks like:
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
abs_add64(dword a[], dword n, qword n64)
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

static big_number_t *
bn_abs_add(big_number_t *a, big_number_t *b)
{
	big_number_t *pmax = NULL;
	big_number_t *pmin = NULL;

	if (abs_gt(a, b)) {
		pmax = a;
		pmin = b;
	} else {
		pmax = b;
		pmin = a;
	}

	big_number_t *c = new_big_number(pmax->size + 1, 0x0);
	if (c == NULL)
		return NULL;

	/* copy the max one to dst */
	for (dword i = 0; i < pmax->size; i++)
		c->data[i] = pmax->data[i];

	/* add the min one to dst */
	for (dword i = 0; i < pmin->size; i++)
		abs_add64(c->data + i, c->size - i, (qword)(pmin->data[i]));

	c->size = bn_get_valid_size(c);

	return c;
}

static big_number_t *
bn_abs_sub(big_number_t *a, big_number_t *b)
{
	big_number_t *c = NULL;

	big_number_t *pmax = NULL;
	big_number_t *pmin = NULL;

	if (abs_gt(a, b)) {
		pmax = a;
		pmin = b;
	} else {
		pmax = b;
		pmin = a;
	}

	/* allocate a temp big number */
	big_number_t *t = new_big_number(pmax->size, 0x0);
	if (t == NULL)
		return NULL;

	/* copy the min one to the temp big number */
	for (dword i = 0; i < pmin->size; i++)
		t->data[i] = pmin->data[i];
	/* pick up t->data[-1] then set sign */
	t->data[pmax->size - 1] = sign_neg;

	/* get complement of the temp big number */
	for (dword i = 0; i < pmax->size; i++)
		t->data[i] = ~(t->data[i]);
	abs_add64(t->data, t->size, (qword)0x1);

	/* now execute big number addition */
	c = bn_abs_add(pmax, t);
	if (c == NULL)
		goto done;

	/*
	 * erase the sign at c->data[c->size - 2] == (dword)~0 == 0xffffffff
	 * please be noted that "c->size - 2 == pmax->size - 1" in fact
	 */
	*(c->data + c->size - 2) = 0x0;

	c->size = bn_get_valid_size(c);
done:
	free_big_number(t);
	return c;
}

static big_number_t *
bn_abs_mul(big_number_t *a, big_number_t *b)
{
	big_number_t *c = new_big_number(a->size + b->size, 0x0);
	if (c == NULL)
		return NULL;

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
			abs_add64(dst, c->size - (i + j), n64);
		}
	}

	c->size = bn_get_valid_size(c);

	return c;
}

/**
 * Left shift a big number bit by bit, offset in [0, 31] bits are shifted
 */
static big_number_t *
_abs_shl_offset(big_number_t *p, byte offset)
{
	/* offset must be in [0, 31] */
	if (offset > 31)
		return NULL;

	big_number_t *t = new_big_number(p->size + 1, (dword)(1 << offset));
	if (t == NULL)
		return NULL;

	big_number_t *c = bn_abs_mul(p, t);
	if (c == NULL)
		goto done;

	c->size = bn_get_valid_size(c);
done:
	free_big_number(t);
	return c;
}

/**
 * Left shift a big number dword by dword
 */
static big_number_t *
_abs_shl_segment(big_number_t *p, dword segment)
{
	big_number_t *c = new_big_number(p->size + segment, 0x0);
	if (c == NULL)
		return NULL;

	for (dword i = 0; i < p->size; i++)
		c->data[i + segment] = p->data[i];

	return c;
}

/**
 * Left shift a bit number by n bits
 */
big_number_t *
abs_shl(big_number_t *a, qword n)
{
	qword segment = n / 32;
	qword offset  = n % 32;

	big_number_t *t = _abs_shl_segment(a, (dword)segment);
	if (t == NULL)
		return NULL;

	big_number_t *c = _abs_shl_offset(t, (byte)offset);
	if (c == NULL)
		goto done;

	c->size = bn_get_valid_size(c);
done:
	free_big_number(t);
	return c;
}

/**
 * Set big number a with big number b, i.e. a = b.
 * Note that we free the old a once a is set
 */
#define BN_SET(a, b)	do { \
	big_number_t *a##b = (a); \
	(a) = (b); \
	free_big_number(a##b); \
} while (0)

static big_number_t *
bn_abs_fac(big_number_t *a)
{
	big_number_t *i = new_big_number(a->size + 1, 0x1);
	if (i == NULL)
		return NULL;

	/* return 1 if a is 0x0 */
	if (is_zero(a)) {
		i->size = bn_get_valid_size(i);
		return i;
	}

	/*
	 * implement factorial by leveraging a pretty simple algorithm
	 * in the following:
	 *
	 *     sum = 1;
	 *     for (i = 1; i <= a; i++) {
	 *         mus = sum * i;
	 *         sum = mus;
	 *     }
	 *
	 */
	big_number_t *sum = new_big_number(0x2, 0x1); /* sum = 1 */
	if (sum == NULL)
		return NULL;

	while (abs_le(i, a)) {
		big_number_t *mus = bn_abs_mul(sum, i); /* mus = sum * i */
		if (mus == NULL) {
			free_big_number(sum);
			free_big_number(i);
			return NULL;
		}

		BN_SET(sum, mus); /* sum = mus */

		abs_add64(i->data, i->size, (qword)0x1); /* i++ */
	}

	free_big_number(i);

	sum->sign = sign_pos;
	sum->size = bn_get_valid_size(sum);
	return sum;
}

static big_number_t *
bn_abs_pow(big_number_t *a, dword n)
{
	big_number_t *sum = new_big_number(a->size + 1, 0x1);
	if (sum == NULL)
		return NULL;

	for (dword i = 0; i < n; i++) {
		big_number_t *mus = bn_abs_mul(sum, a);
		if (mus == NULL) {
			free_big_number(sum);
			return NULL;
		}

		BN_SET(sum, mus); /* sum = mus */
	}

	sum->sign = sign_pos;
	sum->size = bn_get_valid_size(sum);
	return sum;
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

big_number_t *
bn_add(big_number_t *a, big_number_t *b)
{
	sign_t sign = sign_pos;
	big_number_t *c = NULL;

	if (a->sign == sign_neg && b->sign == sign_neg) {
		sign = sign_neg;
		c = bn_abs_add(a, b);
		goto done;
	}

	if (a->sign == sign_pos && b->sign == sign_pos) {
		sign = sign_pos;
		c = bn_abs_add(a, b);
		goto done;
	}

	if (a->sign == sign_neg && b->sign == sign_pos) {
		sign = abs_gt(a, b) ? sign_neg : sign_pos;
		c = bn_abs_sub(a, b);
		goto done;
	}

	if (a->sign == sign_pos && b->sign == sign_neg) {
		sign = abs_gt(b, a) ? sign_neg : sign_pos;
		c = bn_abs_sub(a, b);
		goto done;
	}

done:
	if (c == NULL)
		return NULL;

	c->sign = sign;
	return c;
}

big_number_t *
bn_sub(big_number_t *a, big_number_t *b)
{
	sign_t sign = sign_pos;
	big_number_t *c = NULL;

	if (a->sign == sign_neg && b->sign == sign_pos) {
		sign = sign_neg;
		c = bn_abs_add(a, b);
		goto done;
	}

	if (a->sign == sign_pos && b->sign == sign_neg) {
		sign = sign_pos;
		c = bn_abs_add(a, b);
		goto done;
	}

	if (a->sign == sign_neg && b->sign == sign_neg) {
		sign = abs_gt(a, b) ? sign_neg : sign_pos;
		c = bn_abs_sub(a, b);
		goto done;
	}

	if (a->sign == sign_pos && b->sign == sign_pos) {
		sign = abs_lt(a, b) ? sign_neg : sign_pos;
		c = bn_abs_sub(a, b);
		goto done;
	}

done:
	if (c == NULL)
		return NULL;

	c->sign = sign;
	return c;
}

big_number_t *
bn_mul(big_number_t *a, big_number_t *b)
{
	sign_t sign = sign_pos;
	big_number_t *c = NULL;

	sign = (a->sign == b->sign) ? sign_pos : sign_neg;

	c = bn_abs_mul(a, b);
	if (c == NULL)
		return NULL;

	c->sign = sign;
	return c;
}

big_number_t *
bn_fac(big_number_t *a)
{
	if (a->sign == sign_neg)
		return NULL;
	return bn_abs_fac(a);
}

big_number_t *
bn_pow(big_number_t *a, dword n)
{
	sign_t sign = sign_pos;
	big_number_t *c = NULL;

	/*
	 * XXX: You may ask why n is not defined as big_number_t but dword for
	 *      the time being. Well, we shall have more work to tell n is odd
	 *      or even if we define n as big_number_t.
	 */
	if (a->sign == sign_neg)
		sign = (n % 2 == 0) ? sign_pos : sign_neg;

	c = bn_abs_pow(a, n);
	if (c == NULL)
		return NULL;

	c->sign = sign;
	return c;
}

static char *
str2bn_align(char *s)
{
	size_t n = strlen(s);
	int m = (n % 8 != 0) ? (8 - (n % 8)) : 0;
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
	sign_t sign = sign_pos;

	if (s == NULL || *s == '\0')
		return NULL;

	size_t n = strlen(s);
	char *p = (char *)s;

	if (*p == '+') {
		p++;
		sign = sign_pos;
	}

	if (*p == '-') {
		p++;
		sign = sign_neg;
	}

	if (*p == '0' && *(p + 1) == 'x') {
		p += 2;
		n -= 2;
	}

	big_number_t *c = (big_number_t *)malloc(sizeof(big_number_t));
	if (c == NULL) /* malloc error */
		return NULL;

	c->sign = sign;
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

	c->size = bn_get_valid_size(c);

	return c;
}

/**
 * convert big number to string
 */
char *
bn2str(big_number_t *bn)
{
	size_t n = bn->size * 8;
	char *p = (char *)malloc(sizeof(char) * (n + 4));
	if (p == NULL)
		return NULL;

	memset(p, 0, n + 4);
	*(p + 0) = ' ';
	*(p + 1) = '0';
	*(p + 2) = 'x';

	if (bn->sign == sign_neg)
		*(p + 0) = '-';

	size_t m = bn->size;
	for (int i = bn->size - 1; i >= 0; i--) {
		if (bn->data[i] != 0x0)
			break;
		m--;
	}

	if (m == 0) {
		*(p + 3) = '0';
		return p;
	}

	for (int i = m - 1; i >= 0; i--) {
		char buf[9] = { 0 };
		snprintf(buf, sizeof(buf), "%08x", bn->data[i]);
		strncat(p, buf, n - 2);
	}

	return p;
}
