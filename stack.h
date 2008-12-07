#ifndef STACK_H
#define STACK_H

#include "cube.h"


/*
 * This is a custom stack data structure that is meant to take as one structure the
 * following information:
 * a cube type (120 length string)
 * an integer indicating what turn formed this cube
 * an integer indicating the distance from the start
 */

/*
 * This struct is what data goes in each slot in the stack
 */
typedef struct {
    cube cube_data;
    int turn;
    int distance;
} qdata;

/*
 * This struct is each block of data.  This should be large so
 * we don't do many mallocs
 */
#define BLOCKLEN 100
typedef struct BLOCK {
    struct BLOCK *leftlink;
    /* no need for a right link */
    qdata data[BLOCKLEN];
} block;

/*
 * This structure holds stack specific information.
 * malloc enough space for this and pass a reference
 * to this to all stack methods
 */
typedef struct {
    block *rightblock;
    int rightindex; /* index into last block, points to last element */
    long length;
} stacktype;

/*
 * This pushes a cube to the stack,
 * pass in a copy of the cube type, the turn used to produce this
 * cube, and the distance.  It will be pushed onto the stack
 */
int stack_push(stacktype *stack, cube cube_to_append, int turn, int distance);

/*
 * This stack works a bit differently.  There are operations to individually
 * peek at the different items at the top of the stack, but the stack remains
 * unchanged.  Then the pop operation doesn't return anything, but just
 * destroys the topmost element.  This is done so that I don't deal with
 * references and all that, most all data transfer is done right on the stack.
 *
 * These could even be macros (look into this XXX)
 */
int stack_peek_cube(stacktype *stack, cube *targetcube);
int stack_peek_turn(stacktype *stack);
int stack_peek_distance(stacktype *stack);

int stack_pop(stacktype *stack);
#endif
