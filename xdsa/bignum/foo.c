/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbignum.h"

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <a> <b>\n", argv[0]);
		return -1;
	}

	big_number_t *a = str2bn(argv[1]);
	big_number_t *b = str2bn(argv[2]);

	if (gt(a, b))
		printf("A > B\n\n");
	else
		printf("A <= B\n\n");

	dump_big_number("BigNumber A", a);
	dump_big_number("BigNumber B", b);

	big_number_t *c = big_number_mul(a, b);
	dump_big_number("  C = A * B", c);

	big_number_t *d = big_number_add(a, b);
	dump_big_number("  D = A + B", d);

	big_number_t *e = big_number_sub(a, b);
	dump_big_number("  E = A - B", e);

	char *pa = bn2str(a);
	char *pb = bn2str(b);
	char *pc = bn2str(c);
	char *pd = bn2str(d);
	char *pe = bn2str(e);
	printf("\n%s == %s * %s\n", pc, pa, pb);
	printf("\n%s == %s + %s\n", pd, pa, pb);
	printf("\n%s == %s - %s\n", pe, pa, pb);
	free(pa);
	free(pb);
	free(pc);
	free(pd);
	free(pe);

	free_big_number(e);
	free_big_number(d);
	free_big_number(c);
	free_big_number(b);
	free_big_number(a);

	return 0;
}
