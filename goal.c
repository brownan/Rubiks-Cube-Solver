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
    int depth = -1;


    /*
     * primitive stack to hold the solution
     */
    int path[GOAL_MAXDEPTH+1];
    path[0] = -1;

    /*
     * The stack
     */
    stack = STACK_NEW;

    /*
     * thinking out loud: this is practically an inf. loop, searching up to 25
     * moves deep is going to take forever.  Consider changing to unconditional
     * loop, maybe it'll save a few instructions?
     */
    while (depth <= GOAL_MAXDEPTH)
    {
        if (stack->length == 0)
        {
            stack_push(stack, solution, -1, 0);
            depth++;
            fprintf(stderr, "Now searching depth %d, total ndoes expanded: %ld,"
                    " nodes traversed: %ld\n", depth, nodecount, popcount);
        }

        /* pop */
        ++popcount;
        stack_peek_cube(stack, &(current.cube_data));
        current.turn = stack_peek_turn(stack);
        current.distance = stack_peek_distance(stack);
        stack_pop(stack);
        
        /* update the path array */
        if (current.distance > 0) {
            path[current.distance - 1] = current.turn;
            path[current.distance] = -1;
        }
        /*
         * Now, if the cube is at our current depth goal, check if it's
         * solved.
         * If we're below current depth, add each turn (except skip tuple)
         * to the stack.   If depth+heuristic is greater than
         * our current depth, it will be thrown out.
         */ 
        if (current.distance == depth) {
            /* is it solved? */
            for (i=0; i<20; i+=1) {
                if (current.cube_type[i*7] != i)
                    break;
            }
            if (i == 20) {
                /*
                 * DINGDINGDINGDING We've found a solution
                 */
                printf("Solution found!\n");
                i=0;
                while (path[i] != -1)
                    printf("%d  ", path[i]);

                goto solve_cleanup;
            }
        } else {
            /*
             * not at depth yet, turn this cube all about, applying heuristics
             * pruning
             */
            /* TODO */
        }
    }


    /*
     * free memory at the end before returning
     */
solve_cleanup:
    /* XXX MAKE SURE STACK IS EMPTY (could still leak memory) */
    free(stack);
    return 1;
}
