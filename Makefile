CC=gcc
CFLAGS=-O -Wall -pedantic -Werror

SRCS=cube.c stack.c common.c cornertable.c
HEADS=cube.h stack.h common.h cornertable.h
OBJS=cube.o stack.o common.o cornertable.o

all: $(OBJS) tags

tags: $(SRCS) $(HEADS)
	ctags -R .

clean:
	-rm -f *.o
