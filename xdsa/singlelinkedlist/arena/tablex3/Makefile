#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=c99 -m32

all: foo data test

foo: main.o foo.o list.o
	${CC} ${CFLAGS} -o $@ $^

main.o: main.c
	${CC} ${CFLAGS} -c $<

foo.o: foo.c foo.h
	${CC} ${CFLAGS} -c $<

list.o: list.c list.h
	${CC} ${CFLAGS} -c $<

data: data_generator
data_generator: data_generator.sh
	cp $< $@ && chmod 0755 $@ && bash $@

test: test.sh
	cp $< $@ && chmod 0755 $@

clean:
	rm -f *.o

clobber: clean
	rm -rf foo
	rm -rf ./data data_generator test
cl: clobber
