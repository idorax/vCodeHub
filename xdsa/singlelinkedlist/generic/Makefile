#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -m32 -std=c99

all: foo test

foo: foo.o list.o
	${CC} ${CFLAGS} -o $@ $^

foo.o: foo.c
	${CC} ${CFLAGS} -c $<

list.o: list.c list.h
	${CC} ${CFLAGS} -c $<

test: test.ksh
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o

clobber: clean
	rm -f foo
	rm -f test
cl: clobber
