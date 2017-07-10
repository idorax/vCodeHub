/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Dump file in hex format and convert it back. Note it supports to
 * encode/decode with password which can be specified by user.
 *
 * NOTE:
 *      1. base64(1) and binhex module in Python do the similar work.
 *      2. We can also use hexdump(1) or od(1) to dump binary file
 *         in hex format but both hexdump(1) and od(1) don't support
 *         to encode with password. However, this one supports to
 *         encode/decode any file including binary file with password.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

char *g_outfile = NULL;
char *g_password = "@"__FILE__";0x12345ABCDE";

#define O_WIDTH 80 /* default width of output after encoding */

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

static uint16_t
get_secid(char *password)
{
	uint32_t n = strlen(password);
	uint32_t m = 0;
	for (int i = 0; i < n; i++) {
		char c1 = *(password + i);
		char c2 = *(password + n - 1 - i);
		m += c1 * c2 * (i + 1);
	}
	return (m % 0x7fff);
}

static int
bh_encode(int argc, char *argv[])
{
	if (argc != 1)
		return -1;

	/* get secid */
	uint16_t secid = get_secid(g_password);

	char *fbin = argv[0];

	/* get file size */
	struct stat buf;
	if (stat(fbin, &buf) < 0) {
		fprintf(stderr, "fail to stat file %s\n", fbin);
		return -1;
	}

	/* open file */
	int rc = 0;
	int fd = open(fbin, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "fail to open file %s\n", fbin);
		return -1;
	}

	/* NOTE: size_t is __off_t (long int), so we use 16 bytes right here */
	int count = O_WIDTH;
	int n = printf("%016lx", buf.st_size);
	count -= n;

	/* read the file byte by byte, and print as hex */
	for (size_t i = 0; i < buf.st_size; i++) {
		uint8_t b = '\0';
		uint8_t nbytes = 1;
		rc = read(fd, (void *)&b, nbytes);
		if (rc == nbytes) {
			n = printf("%04x", (b << 4) + secid);
			count -= n;
			if (count == 0) {
				printf("\n");
				count = O_WIDTH;
			}
		} else {
			fprintf(stderr, "FATAL: fail to read %dB\n", nbytes);
			goto done;
		}
	}

	/* append some dummy data as signature */
	for (int i = 0; i < count / 4; i++)
		printf("%04x", ((i << 4) ^ (secid >> 4)) & 0xffff);
	printf("\n");

done:
	close(fd);
	return rc;
}

static int
bh_decode(int argc, char *argv[])
{
	if (argc != 1)
		return -1;

	char *fhex = argv[0];
	int rc = 0;

	/* get secid */
	uint16_t secid = get_secid(g_password);

	/* open file */
	int fd1 = open(fhex, O_RDONLY);
	if (fd1 == -1) {
		fprintf(stderr, "fail to open file %s\n", fhex);
		return -1;
	}

	/* read the 1st 16 chars which is the file size */
	char buf[32] = { 0 };
	rc = read(fd1, (void *)buf, 16);
	if (rc != 16)
		goto done1;

	size_t fsize = atoll16(buf);

	/* open the outfile */
	if (g_outfile == NULL) {
		fprintf(stderr, "outfile is not specified\n");
		goto done1;
	}

	int fd2 = open(g_outfile, O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (fd2 == -1) {
		fprintf(stderr, "fail to open file %s\n", g_outfile);
		goto done1;
	}

	/* read the left from fd1 then write to fd2 */
	for (size_t i = 0; i < fsize; i++) {
		buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = '\0';

		/* read 4 bytes from fd1 */
		uint8_t nbytes = 4;
		rc = read(fd1, (void *)buf, nbytes);
		if (rc != nbytes) {
			fprintf(stderr, "fail to read %dB from fd %d\n",
			    nbytes, fd1);
			goto done2;
		}

		/* if buf[] includes '\n', read one more byte */
		uint8_t index = nbytes;
		for (uint8_t j = 0; j < nbytes; j++) {
			if (buf[j] == '\n') {
				index = j;
				break;
			}
		}
		if (index != nbytes) {
			for (uint8_t j = index; j < nbytes - 1; j++)
				buf[j] = buf[j+1];

			rc = read(fd1, &buf[3], 1);
			if (rc != 1) {
				fprintf(stderr, "fail to read 1B from fd %d\n",
				    fd1);
				goto done2;
			}
		}

		/* then write 1 byte to fd2 */
		uint32_t n = 0;
		sscanf(buf, "%x", &n);
		uint8_t b = (n - secid) >> 4;
		rc = write(fd2, (void *)&b, 1);
		if (rc != 1) {
			fprintf(stderr, "fail to write 1B to fd %d\n", fd2);
			goto done2;
		}
	}
	rc = 0;

done2:
	close(fd2);
done1:
	close(fd1);
	return rc;
}

static void
usage(char *s)
{
	fprintf(stderr, "Usage: %s [-p password]"
	     " <-e|-d -o outfile> <binfile>\n", s);
	fprintf(stderr, " e.g.: %s -e /bin/ls > /tmp/ls.out\n", s);
	fprintf(stderr, "       %s -o /tmp/ls -d /tmp/ls.out\n", s);
}

int
main(int argc, char *argv[])
{
	char op = 'E';
	int opt = -1;
	while ((opt = getopt(argc, argv, "p:edo:h")) != -1) {
		switch (opt) {
		case 'e':
			op = 'E';
			break;

		case 'd':
			op = 'D';
			break;

		case 'o':
			g_outfile = optarg;
			break;

		case 'p':
			g_password = optarg;
			break;

		case 'h':
			usage(argv[0]);
			return -1;

		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;
	if (argc == 0) {
		char **argv0 = argv - optind;
		usage(*argv0);
		return -1;
	}

	int rc = (op == 'E') ? bh_encode(argc, argv) : bh_decode(argc, argv);
	return rc;
}
