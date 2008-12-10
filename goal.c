#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "cube.h"

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
    int i, j;
    stacktype *stack;
    int depth = -1;

    /*
     * Vars declared used later by heuristics
     */
    qdata turns[18];
    int heuristics[18];
    int numturns;
    int heuristic;
    /* int heu2; */
    int hash;
    qdata temp;
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
            numturns = 0;
            for (i=0; i<18; ++i) {
                
                /* Determine if we should skip this turn */
                if (current.turn != -1 && SHOULDIAVOID(i, current.turn)) {
                    continue;
                }
                
                /*
                 * make a copy for this turn and turn it
                 */
                memcpy(turned, current.cube_data, CUBELEN);
                cube_turn(turned, i);

                /*
                 * Now get the heuristic value for this cube
                 */
                heuristic = 0;
                hash = corner_hash(turned);
                heuristic = (hash&1 ? \
                        (cornertable[(hash-1)/2] >> 4) : \
                        (cornertable[hash/2] & 15) );
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

                /* The turned node has distance-plus-cost value
                 * f(x) = g(x) + h(x)
                 * g(x) = current.distance+1   is the path cost to this node
                 *      (current.distance is the path length to the current
                 *      node, and turned is current's successor)
                 * h(x) = heuristic   is the estimated path cost from this node
                 *                    to the goal
                 * h(x) NEVER UNDERESTIMATES THE GOAL! THIS IS KEY.
                 *      Since we KNOW that from here the goal is
                 *      AT LEAST h(x) moves away, we can throw some
                 *      turns away
                 * 
                 * If f(x) is greater than depth (our current threshold of
                 * search for the iterative deepening A* search) then we don't
                 * add it to the stack. BAM!
                 */
                int f = heuristic + current.distance + 1;
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
                turns[numturns].cube_data = 
                /*
                 * Note to self: I'm stopping here for now.  I want to
                 * re-work this since the existing code calls uses pointers
                 * and isn't expensive, but here, qdata holds the actual
                 * cube string.
                 * I need to re-think this somehow.  I'm thinking putting the
                 * turns right into the qdata array as they're generated
                 * (overwriting them if we need to prune them), and then
                 * sorting an array of pointers to the qdatas?
                 * I'll do this later
                 */
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
