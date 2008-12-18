CC=gcc
CFLAGS=-Wall -pedantic -Werror -O3
# Pick and choose debugging compiler flags:
# -g  debugging
# -fprofile-arcs -ftest-coverage -pg  profiling
# -DDEBUG_ASSERTS turn on assertation  checks
# -DPROFILE_MODE  stops program after 10mil hashed cubes in corner generation

SRCS=cube.c stack.c common.c cornertable.c main.c goal.c edgetable.c
HEADS=cube.h stack.h common.h cornertable.h goal.h edgetable.h
OBJS=cube.o stack.o common.o cornertable.o goal.o edgetable.o

all: main tags

cleanall: clean all

# objects depend on all headers, as well as their own source file (implied)
# also, remake if the makefile itself changes
$(OBJS): $(HEADS) Makefile

# main depends on all objects, and its own source
main: $(OBJS) main.c
	gcc $(CFLAGS) $(OBJS) main.c -o main

tags: $(SRCS) $(HEADS)
	-ctags -R .

clean:
	-rm -f *.o main *.gcno *.gcov *.gcda gmon.out
