/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Dump binary file in hex format and convert it back to binary file.
 *
 * NOTE:
 *      1. base64(1) and binhex module in Python do the similar work.
 *      2. We can also use hexdump(1) or od(1) to dump binary file
 *         in hex format.
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

static uint32_t
get_secid(char *password)
{
	uint32_t n = strlen(password);
	uint32_t m = 0;
	for (int i = 0; i < n; i++) {
		char c1 = *(password + i);
		char c2 = *(password + n - 1 - i);
		m += c1 * c2 * (i + 1);
	}
	return m % 0x7fff;
}

static int
bh_encode(int argc, char *argv[])
{
	if (argc != 1)
		return -1;

	/* get secid */
	uint8_t secid = get_secid(g_password);

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

	printf("%016lx", buf.st_size); /* size_t is __off_t (long int) */

	/* read the file byte by byte, and print as hex */
	for (size_t i = 0; i < buf.st_size; i++) {
		uint8_t b = '\0';
		rc = read(fd, (void *)&b, 1);
		if (rc == 1) {
			printf("%04x", (b << 4) + secid);
		} else {
			fprintf(stderr, "FATAL: fail to read 1B\n");
			goto done;
		}
	}
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
	uint8_t secid = get_secid(g_password);

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

	/* read the left every 4 bytes from fd1 and write to fd2 */
	for (size_t i = 0; i < fsize; i++) {
		buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = '\0';

		/* read from fd1 */
		rc = read(fd1, (void *)buf, 4);
		if (rc != 4) {
			fprintf(stderr, "fail to read 2B from fd %d\n", fd1);
			goto done2;
		}

		/* write to fd2 */
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
