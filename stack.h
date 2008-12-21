#ifndef STACK_H
#define STACK_H

#include "cube.h"



/*
 * This struct is what data goes in each slot in the stack
 */
typedef struct {
    cube_type cube_data;
    int turn;
    int distance;
} qdata;

/*
 * This struct is each block of data.  This should be large so
 * we don't do many mallocs
 */
#define BLOCKLEN 10000
typedef struct BLOCK {
    struct BLOCK *leftlink;
    /* no need for a right link */
    qdata data[BLOCKLEN];
} block;

/*
 * This structure holds stack specific information.
 */
typedef struct {
    block *rightblock;
    int rightindex; /* index into last block, points to last element */
    long length;
} stacktype;

/*
 * Returns a newly allocated stacktype.  Use this to make a new stack
 */
#define STACK_NEW calloc(sizeof(stacktype), 1)

int stack_push(stacktype *stack, const char *cube_to_append, int turn, int distance);

int stack_peek_cube(stacktype *stack, cube_type *targetcube);
int stack_peek_turn(stacktype *stack);
int stack_peek_distance(stacktype *stack);

int stack_pop(stacktype *stack);
#endif
