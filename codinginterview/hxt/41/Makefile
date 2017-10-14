#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	=  gcc
CFLAGS	+= -g -Wall -std=c99

all: foo

foo: foo.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o

clobber: clean
	rm -f foo
cl: clobber
