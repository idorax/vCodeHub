/*
 * Copyright (C) 2015, 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * sudorax - A simple utility to encode/decode text protected (e.g. password)
 *
 * NOTE:
 *	If the encoded text is done by user A, and it can be decoded
 *	by user A only. The default lease of encoded text is 180s.
 *
 *	In addition, please be noted that it doesn't support to encode
 *	binary file.
 *
 * o How to Build it
 *   Solaris : gcc -m64           -DMAGICNUM="N*M" -g -Wall \
 *             -std=gnu99 -o sudorax sudorax.c
 *   Linux   : gcc -m32 -D__LINUX -DMAGICNUM="N*M" -g -Wall \
 *             -std=gnu99 -o sudorax sudorax.c
 *
 * o Usage Example
 *   By default,
 *       host1$ ./sudorax -e E
 *       010110010110001000000011100100010000000000111010100011010101110001011001010110001001000111000011
 *       host2$ ./sudorax -d \
 *       010110010110001000000011100100010000000000111010100011010101110001011001010110001001000111000011
 *       E
 *
 *   If -D_USE_HEX is spedified,
 *       host1$ ./sudorax -e foo
 *       596215cc003ab4c45958a41f59580ce7595775a6
 *       host2$ ./sudorax -d 596215cc003ab4c45958a41f59580ce7595775a6
 *       foo
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>

#ifdef __LINUX			/* Linux */
#include <string.h>
#else /* Solaris */
#include <strings.h>
#endif

#ifdef __LINUX
typedef unsigned long long	uint64_t;
typedef unsigned int		uint32_t;
typedef unsigned short		uint16_t;
typedef unsigned char		uint8_t;
#endif

#define KEY_LEASE	180		/* lease of encoded text */
#ifndef MAGICNUM
#define MAGICNUM	(0x9741)	/* magic num to decode/encode */
#endif

uint32_t g_magicnum = MAGICNUM;

static uint64_t
power(int32_t n, uint32_t m)
{
	uint64_t sum = 1;
	for (uint32_t i = 0; i < m; i++)
		sum *= n;
	return sum;
}

static uint64_t
atoll16(char *s)
{
	size_t sz = strlen(s);
	char *p = s;

	uint64_t n = 0;
	for (size_t i = 0; i < sz; i++) {
		char c = *(p + i);

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
	size_t sz = strlen(s);
	char *p = s;

	uint64_t n = 0;
	for (size_t i = 0; i < sz; i++) {
		char c = *(p + i);
		if (c >= '0' && c <= '9')
			c -= '0';

		n += c * power(10, sz - i - 1);
	}

	return n;
}

static uint32_t
get_secid()
{
	struct passwd *pwd = NULL;
	if ((pwd = getpwuid(getuid())) == NULL)
		return 0;

	uint32_t n = 0;
	uint32_t i = 0;
	char *p = pwd->pw_name;
	while (*p != '\0') {
		n += *p;
		n += (i * g_magicnum) % *p;
		p++;
		i++;
	}

	return n;
}

#ifndef  _USE_HEX
static char *
hexchar2bitstr(char c)
{
	char *map[] = {
		"0000", /* 0 */
		"0001", /* 1 */
		"0010", /* 2 */
		"0011", /* 3 */
		"0100", /* 4 */
		"0101", /* 5 */
		"0110", /* 6 */
		"0111", /* 7 */
		"1000", /* 8 */
		"1001", /* 9 */
		"1010", /* a|A */
		"1011", /* b|B */
		"1100", /* c|C */
		"1101", /* d|D */
		"1110", /* e|E */
		"1111", /* f|F */
		NULL
	};

	int index = 0;
	if (c >= '0' && c <= '9')
		index = c - '0';
	else if (c >= 'a' && c <= 'f')
		index = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		index = c - 'a' + 10;
	else
		index = sizeof(map) / sizeof(char *) - 1;

	return map[index];
}

static char *
str2bitstr(char *s)
{
	size_t n = strlen(s) * 4;

	char *buf = (char *)malloc(sizeof (char) * (n + 1));
	if (buf == NULL) /* error */
		return NULL;
	(void)memset(buf, 0, n + 1);

	size_t i = 0;
	for (char *p = s; *p != '\0'; p++) {
		char *bits = hexchar2bitstr(*p);
		snprintf(buf + i, n, "%s", bits);
		i += 4;
	}

	return buf;
}

static char
bitstr2hexchar(char *s)
{
	uint8_t n = 0;
	for (int i = 0; i < 4; i++) {
		uint8_t bit = s[i] - '0'; /* s[i] is either '0' or '1' */
		if (bit != 0x0 && bit != 0x1)
			return '\0';

		n += ((1 << (3 - i)) * bit);
	}

	if (n >= 0 && n <= 9)
		return ('0' + n);
	else if (n >= 10 && n <= 15)
		return ('a' + n - 10);
	else
		return '\0'; /* should not be reached */
}

static void
do_encode(char *str, uint32_t lease)
{
	time_t ts_now = time(NULL);
	time_t ts1 = ts_now - (g_magicnum << 4);
	time_t ts2 = ts_now + lease;
	time_t key = ts2 - (g_magicnum << 4) + get_secid();

	char buf[16] = { 0 }; /* only 8 bytes are required */

#define ENCODE_CORE(buf, num) do { \
		(void)memset(buf, 0, sizeof(buf)); \
		snprintf(buf, sizeof(buf), "%08lx", num); \
		char *p = str2bitstr(buf); \
		printf("%s", p); \
		free(p); \
	} while (0)

	ENCODE_CORE(buf, ts2);         /* time stamp 1: 32 chars */
	ENCODE_CORE(buf, (ts2 ^ ts1)); /* time stamp 2: 32 chars */

	uint32_t i = 0;
	for(char *p = str; *p != '\0'; p++, i++)
		ENCODE_CORE(buf, *p + key - (i * g_magicnum)); /* each char */
}

static void
do_decode(char *str)
{
	char buf[16] = { 0 }; /* only 8 bytes are required */

	/* 0. firstly get current time stamp */
	time_t ts_now = time(NULL);

	/* 1a. read time stamp 1 : 32 chars */
	for (int i = 0; i < 8; i++)
		buf[i] = bitstr2hexchar(str + 4 * i);
	time_t ts1 = atoll16(buf);

	/* 1b. read time stamp 2 : 32 chars */
	memset(buf, 0, sizeof(buf));
	for (int i = 0; i < 8; i++)
		buf[i] = bitstr2hexchar(str + 32 + 4 * i);
	time_t ts2 = ((time_t)atoll16(buf) ^ ts1) + (time_t)(g_magicnum << 4);

	/* 2a. check current time is out of lease */
	time_t key = 0;
	if (ts_now > ts1)
		key = ~0; /* out of lease, use bad key to decode */
	else
		key = ts1 - (g_magicnum << 4) + get_secid();

	/*
	 * 2b. make sure current time is not invalid in case someone once
	 *     reset the system time to long-long-ago before decoding
	 */
	if (ts_now < ts2)
		key = ~0; /* invalid system time, also use bad key to decode */

	/* 3. decode the left chars, every 32 chars are taken as 1 out-char */
	uint32_t bufsize = strlen(str) / 32;
	char *out = (char *)malloc(bufsize * sizeof(char));
	if (out == NULL)
		return;
	(void)memset(out, 0, bufsize * sizeof(char));

	char buf2[40] = { 0 }; /* only 32 bytes are requied */
	uint8_t  i = 0;
	uint32_t j = 0;
	uint32_t n = 0;
	/* NOTE: the first 32 * 2 chars of str are time stamp */
	for (char *p = str + 32 * 2; *p != '\0'; p++) {
		buf2[i++] = *p;

		if (i == 32) {
			i = 0; /* reset i */

			/* convert 32 bit-chars to 8 hex-chars */
			for (int k = 0; k < 8; k++)
				buf[k] = bitstr2hexchar(buf2 + 4 * k);
			buf[8] = '\0';

			/* decode */
			out[j++] = atoll16(buf) - key + (n * g_magicnum);
			n++;
		}
	}

	(void)printf("%s", out);

	free(out);
}
#else /* _USE_HEX */
static void
do_encode(char *str, uint32_t lease)
{
	time_t ts_now = time(NULL);
	time_t ts1 = ts_now - (g_magicnum << 4);
	time_t ts2 = ts_now + lease;
	time_t key = ts2 - (g_magicnum << 4) + get_secid();

	(void)printf("%08lx", ts2);         /* time stamp 1: 8 chars */
	(void)printf("%08lx", (ts2 ^ ts1)); /* time stamp 2: 8 chars */

	uint32_t i = 0;
	for(char *p = str; *p != '\0'; p++, i++)
		(void)printf("%08lx",
		   *p + key - (i * g_magicnum)); /* each char */
}

static void
do_decode(char *str)
{
	char buf[16] = { 0 }; /* only 8 bytes are required */

	/* 0. firstly get current time stamp */
	time_t ts_now = time(NULL);

	/* 1a. read time stamp 1 : 8 chars */
	for (int i = 0; i < 8; i++)
		buf[i] = *(str + i);
	time_t ts1 = atoll16(buf);

	/* 1b. read time stamp 2 : 8 chars */
	memset(buf, 0, sizeof(buf));
	for (int i = 0; i < 8; i++)
		buf[i] = *(str + 8 + i);
	time_t ts2 = ((time_t)atoll16(buf) ^ ts1) + (time_t)(g_magicnum << 4);

	/* 2a. check current time is out of lease */
	time_t key = 0;
	if (ts_now > ts1)
		key = ~0; /* out of lease, use bad key to decode */
	else
		key = ts1 - (g_magicnum << 4) + get_secid();

	/*
	 * 2b. make sure current time is not invalid in case someone once
	 *     reset the system time to long-long-ago before decoding
	 */
	if (ts_now < ts2)
		key = ~0; /* invalid system time, also use bad key to decode */

	/* 3. decode the left chars, every 8 chars are taken as 1 out-char */
	uint32_t bufsize = strlen(str) / 8;
	char *out = (char *)malloc(bufsize * sizeof(char));
	if (out == NULL)
		return;
	(void)memset(out, 0, bufsize * sizeof(char));

	uint8_t  i = 0;
	uint32_t j = 0;
	uint32_t n = 0;
	/* NOTE: the first 8 * 2 chars of str are time stamp */
	for (char *p = str + 8 * 2; *p != '\0'; p++) {
		buf[i++] = *p;

		if (i == 8) {
			i = 0; /* reset i */

			/* decode */
			out[j++] = atoll16(buf) - key + (n * g_magicnum);
			n++;
		}
	}

	(void)printf("%s", out);

	free(out);
}
#endif

static void
usage(char *s)
{
	(void)fprintf(stderr, "Usage: %s <-d|-e> [-l lease] [-m magicnum] "
		      "<-f <file> | string>\n", s);
	(void)fprintf(stderr, "       -d : decode\n");
	(void)fprintf(stderr, "       -e : encode\n");
}

int
main(int argc, char **argv)
{
	char *prog = argv[0];
	char *str = NULL;

	uint32_t lease = KEY_LEASE;
	char op = '\0';
	int opt = -1;
	char *ftxt = NULL;
	while ((opt = getopt(argc, argv, "edm:l:f:h")) != -1) {
		switch (opt) {
		case 'e':
			op = 'E';
			break;
		case 'd':
			op = 'D';
			break;
		case 'm':
			g_magicnum = (uint32_t)atoll10(optarg);
			break;
		case 'l':
			lease = (uint32_t)atoll10(optarg);
			break;
		case 'f':
			ftxt = optarg;
			break;
		case 'h':
			usage(prog);
			return -1;
		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (ftxt != NULL) {
		/* get file size */
		struct stat buf;
		if (stat(ftxt, &buf) < 0) {
			(void)fprintf(stderr, "fail to stat file %s\n", ftxt);
			return -1;
		}

		size_t fsize = buf.st_size;

		/* alloc a buffer whose length = file size + 1 */
		char *str = (char *)malloc(sizeof(char) * (fsize + 1));
		if (str == NULL) {
			(void)fprintf(stderr, "fail to malloc %lu bytes\n",
				      (unsigned long)(fsize + 1));
			return -1;
		}

		/* read all from file to buffer */
		FILE *fp = fopen(ftxt, "r");
		if (fp == NULL) {
			(void)fprintf(stderr, "fail to open file %s\n", ftxt);
			free(str);
			return -1;
		}

		(void)fseek(fp, 0L, SEEK_SET);
		size_t n = fread(str, 1, fsize, fp);
		if (n != fsize) {
			(void)fprintf(stderr, "fail to read %lu bytes"
				      " from file %s",
				      (unsigned long)(fsize + 1), ftxt);
			free(str);
			fclose(fp);
			return -1;
		}

		fclose(fp);

		/* do encode/decode */
		*(str + fsize) = '\0';
		(op == 'D') ? do_decode(str) : do_encode(str, lease);

		/* free buffer */
		free(str);
	} else {
		if (argc < 1) {
			usage(prog);
			return -1;
		}

		str = argv[0];
		(op == 'D') ? do_decode(str) : do_encode(str, lease);
		(void)printf("\n");
	}

	return 0;
}
