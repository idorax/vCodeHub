#
# Copyright (C) 2017, Vector Li (vlhn@sohu.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99 -m32
INCS	=

TARGET	= heapsort

all: ${TARGET}

heapsort: heapsort.o
	${CC} ${CFLAGS} -o $@ $^

heapsort.o: heapsort.c
	${CC} ${CFLAGS} -c $< ${INCS}

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGET}
cl: clobber
