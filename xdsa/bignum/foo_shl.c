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
	int rc = 0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <a> <n>\n", argv[0]);
		return -1;
	}

	big_number_t *a = str2bn(argv[1]);
	big_number_t *t = str2bn(argv[2]);
	dword n = t->data[0];

	dump("BigNumber A", a);
	dump("BigNumber N", t);

	free_big_number(t);

	big_number_t *b = abs_shl(a, n);
	dump("   B = A<<N", b);

	if (b == NULL) {
		goto done;
		rc = -1;
	}

	char *pa = bn2str(a);
	char *pb = bn2str(b);

	printf("\n%s == %s << 0x%x\n", pb, pa, n);

	free(pa);
	free(pb);

done:
	free_big_number(b);
	free_big_number(a);

	return rc;
}
