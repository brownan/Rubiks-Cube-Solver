CC=gcc
CFLAGS=-Wall -pedantic -Werror -O3
#CFLAGS=-fprofile-arcs -ftest-coverage -pg -DDEBUG_ASSERTS -DPROFILE_MODE

SRCS=cube.c stack.c common.c cornertable.c main.c goal.c
HEADS=cube.h stack.h common.h cornertable.h goal.h
OBJS=cube.o stack.o common.o cornertable.o goal.o

all: main tags

cleanall: clean all

# objects depend on all headers, as well as their own source file (implied)
# also, remake if the makefile itself changes
$(OBJS): $(HEADS) Makefile

# main depends on all objects, and its own source
main: $(OBJS) main.c
	gcc $(CFLAGS) $(OBJS) main.c -o main

tags: $(SRCS) $(HEADS)
	ctags -R .

clean:
	-rm -f *.o main *.gcno *.gcov *.gcda gmon.out
