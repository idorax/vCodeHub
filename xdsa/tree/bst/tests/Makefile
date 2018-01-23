#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=c99

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
