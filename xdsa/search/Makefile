#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99
INCS	=

TARGETS	= binarysearch

all: ${TARGETS}

binarysearch: binarysearch.o
	${CC} ${CFLAGS} -o $@ $^

binarysearch.o: binarysearch.c
	${CC} ${CFLAGS} -c $< ${INCS}

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGETS}
cl: clobber
