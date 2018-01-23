#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99
INCS	=

TARGET	= foo

all: ${TARGET}

foo: foo.o libstack.o
	${CC} ${CFLAGS} -o $@ $^

foo.o: foo.c
	${CC} ${CFLAGS} -c $< ${INCS}

libstack.o: libstack.c libstack.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGET}
cl: clobber
