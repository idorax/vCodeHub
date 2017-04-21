#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99 -m32
INCS	=

TARGETS	= heapsort \
	  mergesort \
	  radixsort radixsort2 \
	  shellsort \
	  quicksort \
	  bubblesort \
	  straightinsertionsort \
	  simpleselectionsort \
	  bucketsort

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

shellsort: shellsort.o
	${CC} ${CFLAGS} -o $@ $^

shellsort.o: shellsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

quicksort: quicksort.o
	${CC} ${CFLAGS} -o $@ $^

quicksort.o: quicksort.c
	${CC} ${CFLAGS} -c $< ${INCS}

bubblesort: bubblesort.o
	${CC} ${CFLAGS} -o $@ $^

bubblesort.o: bubblesort.c
	${CC} ${CFLAGS} -c $< ${INCS}

straightinsertionsort: straightinsertionsort.o
	${CC} ${CFLAGS} -o $@ $^

straightinsertionsort.o: straightinsertionsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

simpleselectionsort: simpleselectionsort.o
	${CC} ${CFLAGS} -o $@ $^

simpleselectionsort.o: simpleselectionsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

bucketsort: bucketsort.o
	${CC} ${CFLAGS} -o $@ $^

bucketsort.o: bucketsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGETS}
cl: clobber
