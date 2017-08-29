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

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <n>\n", argv[0]);
		return -1;
	}

	big_number_t *a = str2bn(argv[1]);

	dump("BigNumber A", a);

	big_number_t *b = bn_fac(a);
	dump("     B = A!", b);

	if (b == NULL) {
		goto done;
		rc = -1;
	}

	char *pa = bn2str(a);
	char *pb = bn2str(b);

	printf("\n%s == %s!\n", pb, pa);

	free(pa);
	free(pb);

done:
	free_big_number(b);
	free_big_number(a);

	return rc;
}
