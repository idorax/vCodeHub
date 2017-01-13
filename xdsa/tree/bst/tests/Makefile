CC	= gcc
CFLAGS	= -g -Wall

TARGET	= foo1

all: ${TARGET}

foo1: foo1.o libbst.o
	${CC} ${CFLAGS} -o $@ $^
foo1.o: foo1.c
	${CC} ${CFLAGS} -c $<

libbst.o: ../libbst.c ../libbst.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGET}
clr: clobber
