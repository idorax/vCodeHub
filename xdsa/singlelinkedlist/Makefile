#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -m32 -std=c99
TARGET	= reverse \
	  test

all: ${TARGET}

reverse: reverse.c
	${CC} ${CFLAGS} -o $@ $<

test: test.ksh
	cp $< $@
	chmod +x $@

clean:
	rm -f *.o

clobber: clean
	rm -f ${TARGET}
cl: clobber
