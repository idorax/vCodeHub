#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99 -m32
INCS	=

TARGETS	= heapsort \
	  mergesort \
	  radixsort radixsort2

all: ${TARGETS}

heapsort: heapsort.o
	${CC} ${CFLAGS} -o $@ $^

heapsort.o: heapsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

mergesort: mergesort.o
	${CC} ${CFLAGS} -o $@ $^

mergesort.o: mergesort.c
	${CC} ${CFLAGS} -c $< ${INCS}

radixsort: radixsort.o
	${CC} ${CFLAGS} -o $@ $^

radixsort.o: radixsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

radixsort2: radixsort2.o
	${CC} ${CFLAGS} -o $@ $^

radixsort2.o: radixsort2.c
	${CC} ${CFLAGS} -c $< ${INCS}

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGETS}
cl: clobber
