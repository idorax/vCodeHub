#
# Copyright (C) 2017, Vector Li (idorax@126.com)
#

CC	= gcc
CFLAGS	= -g -Wall -std=c99 -m32

TARGET	= foo1 \
	  foo2

all: ${TARGET}

foo1: foo1.o libbst.o
	${CC} ${CFLAGS} -o $@ $^
foo1.o: foo1.c
	${CC} ${CFLAGS} -c $<

foo2: foo2.o libbst.o
	${CC} ${CFLAGS} -o $@ $^
foo2.o: foo2.c
	${CC} ${CFLAGS} -c $<

libbst.o: ../libbst.c ../libbst.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGET}
cl: clobber
