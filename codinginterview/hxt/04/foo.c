/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * Exam 04 which is addressed from page#44 to page#49
 *
 * 1. ASSIGNMENT
 *    Write a function to replace every blank of a string with a specified
 *    string (e.g. '%20'). For example,
 *        Input:  "We are happy."
 *        Output: "We%20are%20happy."
 *
 * 2. INSTRUCTIONS
 *    Assuming that the buffer saving source string is sufficient to store the
 *    destination string replaced. That is, if the source string is "a b", the
 *    buffer size is 6 which can store "a%20b" for sure. Hence, the time
 *    complexity should be O(n) and the space complexity should be O(1).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
replace(char *s, char c, const char *e)
{
	if (s == NULL || *s == '\0' || e == NULL || *e == '\0')
		return;

	size_t m = strlen(e);

	/* set left pointer to be the end of src string */
	size_t nc = 0; /* total number of char c found in src string */
	char *left = NULL;
	for (left = s; *left != '\0'; left++) {
		if (*left == c)
			nc++;
	}

	/* set right pointer to be the end of dst string */
	char *right = left + nc * (m - 1);

	while (left != s - 1 && right != s - 1) {
		if (*left == c) {
			/* move both left and right pointer backward */
			left  -= 1;
			right -= m;

			/* copy const string e to memory @ (right+1) */
			char *p = right + 1;
			char *q = (char *)e;
			while (*q != '\0')
				*p++ = *q++;

			continue;
		}

		*right-- = *left--;
	}
}

static char *
expand(const char *s, const char *e)
{
	if (s == NULL || *s == '\0' || e == NULL || *e == '\0')
		return NULL;

	size_t max = strlen(s) * strlen(e) + 1;

	char *dst = (char *)malloc(max * sizeof (char));
	if (dst == NULL) /* error */
		return NULL;
	memset(dst, 0, max);

	char *p = dst;
	char *q = (char *)s;
	while (*q != '\0')
		*p++ = *q++;

	return dst;
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <e> <string>\n", argv[0]);
		fprintf(stderr, "  e.g. %s \"%%20\" \"Hello world, Orz\"\n",
		        argv[0]);
		return -1;
	}

	char *se = argv[1];
	char *s1 = argv[2];
	char *s2 = expand(s1, se);
	if (s2 == NULL) {
		fprintf(stderr, "failed to expand()\n");
		return -2;
	}

	printf("Before replacing: %s\n", s1);
	replace(s2, ' ', se);
	printf("After  replacing: %s\n", s2);

	free(s2);

	return (0);
}
