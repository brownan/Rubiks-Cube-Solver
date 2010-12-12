CC=gcc
CFLAGS=-Wall -Werror -O3
# These may help a bit:
#CFLAGS+=-fno-stack-protector
#CFLAGS+=-ftree-parallelize-loops=3

# Pick and choose debugging compiler flags:
# -g  debugging
# -fprofile-arcs -ftest-coverage -pg  profiling
# -DDEBUG_ASSERTS turn on assertation  checks
# -DPROFILE_MODE  stops program after 10mil hashed cubes in corner generation

SRCS=cube.c stack.c common.c cornertable.c main.c goal.c edgetable.c
HEADS=cube.h stack.h common.h cornertable.h goal.h edgetable.h
OBJS=cube.o stack.o common.o cornertable.o goal.o edgetable.o

all: solver tags

cleanall: clean all

# objects depend on all headers, as well as their own source file (implied)
# also, remake if the makefile itself changes
$(OBJS): $(HEADS) Makefile

# solver depends on all objects, and its own source
solver: $(OBJS) main.c
	gcc $(CFLAGS) $(OBJS) main.c -o solver

table_test: $(OBJS) table.o
	gcc $(CFLAGS) $(OBJS) table.o -o table_test

tags: $(SRCS) $(HEADS)
	-ctags -R .

clean:
	-rm -f *.o solver *.gcno *.gcov *.gcda gmon.out
