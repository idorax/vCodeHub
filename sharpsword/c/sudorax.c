/*
 * Copyright (C) 2015, 2017, Vector Li (idorax@126.com)
 */

/**
 * sudorax - A simple utility to encode/decode text protected (e.g. password)
 *
 * NOTE:
 *	If the encoded text is done by user A, and it can be decoded
 *	by user A only. The default lease of encoded text is 600s.
 *
 * o How to Build it
 *   Solaris : gcc -m64           -DMAGICNUM="N*M" -g -Wall -o sudorax sudorax.c
 *   Linux   : gcc -m32 -D__LINUX -DMAGICNUM="N*M" -g -Wall -o sudorax sudorax.c
 *
 * o Usage Example
 *   host1$ ./sudorax -e foo
 *   58a2a0dd58a1ab1158a19b9158a18c08
 *   host2$ ./sudorax -d 58a2a0dd58a1ab1158a19b9158a18c08
 *   foo
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <stdlib.h>

#ifdef __LINUX	/* Linux */
#include <string.h>
#else		/* Solaris */
#include <strings.h>
#endif

#ifdef __LINUX
typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
#endif

#define KEY_LEASE	600		/* lease of encoded password */
#define BUF_SIZE	(1024 * 1024)	/* 1M */
#ifndef MAGICNUM
#define MAGICNUM	(0x9741)	/* magic num to decode/encode */
#endif

static uint64_t
power(int32_t n, uint32_t m)
{
	uint32_t i;
	uint64_t sum = 1;

	for (i = 0; i < m; i++)
		sum *= n;

	return sum;
}

static uint64_t
atoll16(char *s)
{
	uint64_t n = 0;
	size_t sz = strlen(s);
	char *p = s;
	char c;
	int i = 0;

	for (i = 0; i < sz; i++) {
		c = *(p + i);

		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'F')
			c = c + 10 - 'A';
		else if (c >= 'a' && c <= 'f')
			c = c + 10 - 'a';

		n += c * power(16, sz - i - 1);
	}

	return n;
}

static uint64_t
atoll10(char *s)
{
	uint64_t n = 0;
	size_t sz = strlen(s);
	char *p = s;
	char c;
	int i = 0;

	for (i = 0; i < sz; i++) {
		c = *(p + i);

		if (c >= '0' && c <= '9')
			c -= '0';

		n += c * power(10, sz - i - 1);
	}

	return n;
}

static uint32_t
get_secid()
{
	char *p = NULL;
	uint32_t i = 0;
	uint32_t n = 0;
	struct passwd *pwd = NULL;

	if ((pwd = getpwuid(getuid())) == NULL)
		return 0;

	p = pwd->pw_name;
	while (*p != '\0') {
		n += *p;
		n += (i * (MAGICNUM)) % *p;
		p++;
		i++;
	}

	return n;
}

static void
do_encode(char *str, uint32_t lease)
{
	char c = '\0';
	char *p = str;
	time_t ts = time(NULL) + lease;
	time_t key = ts - ((MAGICNUM) << 4) + get_secid();

	(void) printf("%08lx", ts); /* time stamp : 8 chars */

	int i = 0;
	while (*p != '\0') {
		c = *p;
		p++;
		(void) printf("%08lx", c + key - (i * 41 * 97)); /* each char */
		i++;
	}

	(void) printf("\n");
}

static void
do_decode(char *str, int reverse)
{
	char *out = NULL;
	char *out2 = NULL;

	char c = '\0';
	char *p = str;
	time_t key = 0;
	time_t ts = 0;
	time_t ts_now = time(NULL);

	int n = 0;
	int i = 0;
	int j = 0;
	char buf[9] = {0};

	/* read time stamp : 8 chars */
	for (i = 0; i < 8; i++)
		buf[i] = *(p + i);

	/* check current time is out of lease */
	ts = atoll16(buf);
	if (ts_now > ts)
		key = ~0; /* out of lease, use bad key to decode */
	else
		key = ts - ((MAGICNUM) << 4) + get_secid();

	/* decode the left chars, every 8 chars are handled */
	if ((out = (char *)malloc(BUF_SIZE * sizeof (char))) == NULL)
		goto cleanup;
	(void) memset(out, 0, BUF_SIZE * sizeof (char));

	i = 0;
	p += 8;
	while (*p != '\0') {
		c = *p;
		buf[i++] = c;
		if (i == 8) {
			i = 0;
			out[j++] = (char)(atoll16(buf) - key + (n * 41 * 97));
			n++;
		}
		p++;
	}

	p = out; /* reverse is 0 by default */
	if (reverse != 0) {
		i = strlen(out);

		if ((out2 = (char *)malloc((i + 1) * sizeof (char))) == NULL)
			goto cleanup;
		(void) memset(out2, 0, (i + 1) * sizeof (char));

		j = 0;
		p = out;
		while (i > 0) {
			out2[j] = *(p + i - 1);
			i--;
			j++;
		}

		p = out2;
	}

	(void) printf("%s\n", p);

cleanup:
	if (out != NULL)
		free(out);
	if (out2 != NULL)
		free(out2);
}

static void
usage(char *s)
{
	(void) fprintf(stderr, "Usage: %s <[-r] -d|-e> <string>\n", s);
	(void) fprintf(stderr, "       -d : decode\n");
	(void) fprintf(stderr, "       -e : encode\n");
	(void) fprintf(stderr, "       -r : drvs when decoding\n");
}

int
main(int argc, char **argv)
{
	char *prog = argv[0];
	char *str = NULL;

	int drvs = 0;
	uint32_t lease = KEY_LEASE;
	char op = '\0';
	int opt = -1;
	while ((opt = getopt(argc, argv, "redl:h")) != -1) {
		switch (opt) {
			/* -r: reverse string when decoding */
			case 'r': drvs = 1; break;
			case 'e': op = 'E'; break;
			case 'd': op = 'D'; break;
			/* -l: just for debugging */
			case 'l': lease = (uint32_t)atoll10(optarg); break;
			case 'h': usage(prog); return -1;
			default:  break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		usage(prog);
		return -1;
	}

	str = argv[0];
	(op == 'D') ? do_decode(str, drvs) : do_encode(str, lease);

	return 0;
}
