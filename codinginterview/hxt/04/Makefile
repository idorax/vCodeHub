#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

CC	=  gcc
CFLAGS	+= -g -Wall -std=c99

all: foo test

foo: foo.c
	${CC} ${CFLAGS} -o $@ $<

test: test.sh
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o

clobber: clean
	rm -f foo
	rm -f test
cl: clobber
