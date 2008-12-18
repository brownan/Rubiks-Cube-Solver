#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "cube.h"
#include "goal.h"
#include "cornertable.h"
#include "edgetable.h"
#include "common.h"

/*
 * as always, see the header file for more comments
 */

int goal_solve(const char *scrambled, const char *solved,
        const unsigned char *cornertable,
        const unsigned char *edgetable1)
{
    /*
     * declaring all variables upfront
     */
    long nodecount = 0; /* nodes generated */
    long popcount = 0; /* nodes traversed */
    
    qdata current;
    int i, j;
    stacktype *stack;
    int depth = -1;

    /*
     * Vars declared used later by heuristics and turns
     */
    /* Each turn is put into the next slot in here (turns[numturns]) */
    qdata turns[18];
    qdata *turns_sorted[18]; /* points to elements of turns[] */
    int heuristics[18]; /* heuristics values of the corresponding elements */
    /* If the turn isn't eliminated by any heuristic, this is incremented */
    int numturns;
    /* for swapping */
    qdata *tempqdata;
    
    /* Used to store the heuristic of the current turn */
    int heuristic;
    int hash;
    int heu2;

    int f;


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
            stack_push(stack, scrambled, -1, 0);
            depth++;
            fprintf(stderr, "Searching depth %d, Nodes expanded: %ld,"
                    " Nodes traversed: %ld\n", depth, nodecount, popcount);
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
         */ 
        if (current.distance == depth) {
            /* is it solved? */
            if (memcmp(current.cube_data, solved, CUBELEN) == 0) {
                /*
                 * DINGDINGDINGDING We've found a solution
                 */
                printf("Solution found!\n");
                i=0;
                while (path[i] != -1)
                    printf("%d  ", path[i++]);

                printf("\n");
                goto solve_cleanup;
            }
        } else {
            /*
             * not at depth yet, turn this cube all about, applying heuristics
             * pruning
             */
            numturns = 0;
            for (i=0; i<18; ++i) {
                
                /* Determine if we should skip this turn */
                if (current.turn != -1 && SHOULDIAVOID(i, current.turn)) {
                    continue;
                }
                
                /*
                 * make a copy for this turn and turn it
                 */
                memcpy(turns[numturns].cube_data, current.cube_data, CUBELEN);
                cube_turn(turns[numturns].cube_data, i);
                turns[numturns].turn = i;
                turns_sorted[numturns] = &turns[numturns];

                /*
                 * Now get the heuristic value for this cube
                 */
                heuristic = 0;

                hash = corner_hash(turns[numturns].cube_data);
                heuristic = TABLE_LOOKUP(cornertable, hash);
#ifdef DEBUG_ASSERTS
                if (heuristic < 0 || heuristic > 11) {
                    fprintf(stderr, "\nWARNING: CORNER HERUISTIC OUT OF BOUNDS\n");
                    hash = *((int *)0x0);
                }
#endif

                /*
                 * Here, we would put the next heuristic value into
                 * heu2
                 * and set heuristic to max(heruistic,heu2)
                 *
                 * repeat as necessary
                 */
                
                hash = edge_hash1(turns[numturns].cube_data);
                heu2 = TABLE_LOOKUP(edgetable1, hash);
                if (heu2 > heuristic) {
                    heuristic = heu2;
                }
#ifdef DEBUG_ASSERTS
                if (heu2 < 0 || heu2 > 10) {
                    fprintf(stderr, "\nWARNING: EDGE HEURISTIC 1 OUT OF BOUNDS\n");
                    hash = *((int *)0x0);
                }
#endif
                

                /* The turned node has distance-plus-cost value
                 * f(x) = g(x) + h(x)
                 * g(x) = current.distance+1   is the path cost to this node
                 *      (current.distance is the path length to the current
                 *      node, and turned is current's successor)
                 * h(x) = heuristic   is the estimated path cost from this node
                 *                    to the goal
                 * h(x) NEVER UNDERESTIMATES THE GOAL! THIS IS KEY.
                 *      Since we KNOW that from here the goal is
                 *      AT LEAST h(x) moves away, we maybe can throw
                 *      this turn away depending on our current search
                 *      depth
                 * 
                 * If f(x) is greater than depth (our current threshold of
                 * search for the iterative deepening A* search) then we don't
                 * add it to the stack. BAM!
                 */
                f = heuristic + current.distance + 1;
                if (f > depth) {
                    /* 
                     * Skip the rest of this
                     * loop iteration
                     * This goes up to the for loop that loops
                     * over each turn id.
                     */
                    continue;
                }

                /*
                 * Now we want to add this turn to the stack, but we're not
                 * going to do that just yet.  We collect all the turns into
                 * this array, then sort them as an additional heuristic that
                 * may or may not be worth the time it takes.
                 */
                heuristics[numturns] = f;
                ++numturns;
                ++nodecount;
            }

            /*
             * Now we have numturns number of elements in the turns array, with
             * turns_sorted corresponding pointers to each one, and
             * corresponding heuristics in the heuristics array.
             * 
             * Below:
             * 1. Sort the turns_sorted and heuristics array descending by the
             * heuristic, and then
             * 2. add them in order to the stack.
             *
             * XXX MAKE SURE THIS IS SORTING IN THE CORRECT ORDER!
             */

            /* Step 1 */
            for (i=1; i<numturns; i++) {
                tempqdata = turns_sorted[i];
                heuristic = heuristics[i]; /* repurposed: temp var */
                j = i - 1;
                while (j >= 0 && heuristics[j] < heuristic) {
                    turns_sorted[j+1] = turns_sorted[j];
                    heuristics[j+1] = heuristics[j];
                    --j;
                }
                turns_sorted[j+1] = tempqdata;
                heuristics[j+1] = heuristic;
            }

            /* step 2: add to stack */
            for (i=0; i<numturns; ++i) {
                stack_push(stack, turns_sorted[i]->cube_data, turns_sorted[i]->turn,
                        current.distance+1);
            }
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
