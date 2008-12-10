#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/*
 * as always, see the header file for more comments
 */

int goal_solve(const char *scrambled, const char *solved,
        const unsigned char *cornertable)
{
    /*
     * declaring all variables upfront
     */
    long nodecount = 0; /* nodes generated */
    long popcount = 0; /* nodes traversed */
    
    qdata current;
    cube_type turned;
    int i;
    stacktype *stack;


    /*
     * primitive stack to hold the solution
     */
    int path[GOAL_MAXDEPTH+1];
    path[0] = -1;

    /*
     * The stack
     */
    stack = malloc(sizeof(stacktype));
    stack->rightblock = NULL; /* so it knows it's empty */




    /*
     * free memory at the end before returning
     */
    /* XXX MAKE SURE STACK IS EMPTY (could still leak memory) */
    free(stack);
    return 1;
}
