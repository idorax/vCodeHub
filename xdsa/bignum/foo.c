/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbignum.h"

static void
dump(char *tag, big_number_t *p)
{
        if (p == NULL)
                return;

        printf("%s : data=%p : size=%d:\t", tag, p, p->size);
        for (dword i = 0; i < p->size; i++)
                printf("0x%08x ", (p->data)[i]);
        printf("\n");
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <a> <b>\n", argv[0]);
		return -1;
	}

	big_number_t *a = str2bn(argv[1]);
	big_number_t *b = str2bn(argv[2]);

        dump("BigNumber A", a);
        dump("BigNumber B", b);
        big_number_t *c = big_number_mul(a, b);
        dump("  C = A * B", c);

	char *pa = bn2str(a);
	char *pb = bn2str(b);
	char *pc = bn2str(c);
	printf("\n%s == %s * %s\n", pc, pa, pb);
	free(pa);
	free(pb);
	free(pc);

        free_big_number(c);
        free_big_number(b);
        free_big_number(a);

        return 0;
}
