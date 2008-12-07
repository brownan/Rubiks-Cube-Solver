CC=gcc
CFLAGS=-O -Wall -pedantic

SRCS=cube.c stack.c
OBJS=cube.o stack.o

all: $(OBJS) tags

tags:
	ctags -R .

