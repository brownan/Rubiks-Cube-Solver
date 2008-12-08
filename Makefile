CC=gcc
CFLAGS=-O2 -Wall -pedantic -Werror

SRCS=cube.c stack.c common.c cornertable.c main.c
HEADS=cube.h stack.h common.h cornertable.h
OBJS=cube.o stack.o common.o cornertable.o

all: main tags

# objects depend on all headers, as well as their own source file
$(OBJS): $(HEADS)

# main depends on all objects, and its own source
main: $(OBJS) main.c
	gcc $(CFLAGS) $(OBJS) main.c -o main

tags: $(SRCS) $(HEADS)
	ctags -R .

clean:
	-rm -f *.o main
