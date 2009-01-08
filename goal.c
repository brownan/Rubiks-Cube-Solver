/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Copyright © 2009 Andrew Brown <brownan@gmail.com>
*/
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
 * goal.c contains routines for actually solving the cube.  Actually only one,
 * the function below takes:
 * 1) the scrambled cube type
 * 2) the solved cube type
 * 3) a pointer to each of the heuristics tables, or null if it doesn't exist
 *
 * The solution is printed to standard out when found
 */
int goal_solve(const char *scrambled, const char *solved,
        const unsigned char *cornertable,
        const unsigned char *edgetable1,
        const unsigned char *edgetable2)
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

    int numtables = 0;

    int solution_found = 0;

    /*
     * primitive stack to hold the solution
     */
    int path[GOAL_MAXDEPTH+1];
    path[0] = -1;


    /*
     * The Depth First Search stack
     */
    stack = STACK_NEW;

    if (cornertable) {
        fprintf(stderr, "Corner table loaded, using corner distance heuristic\n");
        ++numtables;
    }
    if (edgetable1) {
        fprintf(stderr, "Edge table 1 loaded, using edge distance heuristic\n");
        ++numtables;
    }
    if (edgetable2) {
        fprintf(stderr, "Edge table 2 loaded, using edge distance heuristic\n");
        ++numtables;
    }
    fprintf(stderr, "Starting solve method with %d tables\n", numtables);
    if (!numtables){ 
        fprintf(stderr, "WARNING: No tables loaded.  This could take a while\n");
    }


    /*
     * thinking out loud: this is practically an inf. loop, searching up to 25
     * moves deep is going to take forever.  Consider changing to unconditional
     * loop, maybe it'll save a few instructions?
     */
    while (depth <= GOAL_MAXDEPTH)
    {
        if (stack->length == 0)
        {
            if (solution_found)
                goto solve_cleanup;
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
                if (!solution_found) {
                    printf("Solution found!\n");
                    printf("Continuing to look for more solutions at this depth\n");
                    solution_found = 1;
                }
                cube_print_solution(path);
                printf("\n");

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

                if (cornertable) {
                    hash = corner_hash(turns[numturns].cube_data);
                    heuristic = TABLE_LOOKUP(cornertable, hash);
#ifdef DEBUG_ASSERTS
                    if (heuristic < 0 || heuristic > 11) {
                        fprintf(stderr, "\nWARNING: CORNER HERUISTIC OUT OF BOUNDS\n");
                        hash = *((int *)0x0);
                    }
#endif
                }

                /*
                 * Here, we would put the next heuristic value into heu2 and
                 * set heuristic to max(heruistic,heu2)
                 *
                 * repeat as necessary
                 */
                if (edgetable1) {
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
                }
                if (edgetable2) {
                    hash = edge_hash2(turns[numturns].cube_data);
                    heu2 = TABLE_LOOKUP(edgetable2, hash);
                    if (heu2 > heuristic) {
                        heuristic = heu2;
                    }
#ifdef DEBUG_ASSERTS
                    if (heu2 < 0 || heu2 > 10) {
                        fprintf(stderr, "\nWARNING: EDGE HEURISTIC 2 OUT OF BOUNDS\n");
                        hash = *((int *)0x0);
                    }
#endif
                }

                /* The turned node has distance-plus-cost value
                 * f(x) = g(x) + h(x)
                 * g(x) = current.distance+1   is the path cost to this node
                 *      (current.distance is the path length to the current
                 *      node, and turned is current's successor)
                 * h(x) = heuristic   is the estimated path cost from this node
                 *                    to the goal
                 * h(x) NEVER OVERESTIMATES THE GOAL! THIS IS KEY.
                 *      (h(x) will always be less than or equal to the
                 *      actual distance to the goal)
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

/*
 * This function takes a pointer to the path array, and prints to stdout a nice
 * formatted sequence of turns, with letters and all
 */
void cube_print_solution(const int *path)
{
    const int *t = path;
    char face = '\0';
    while (*t != -1) {
        switch (*t) {
            case 0:
            case 6:
            case 12:
                face = 'F';
                break;
            case 1:
            case 7:
            case 13:
                face = 'T';
                break;
            case 2:
            case 8:
            case 14:
                face = 'L';
                break;
            case 3:
            case 9:
            case 15:
                face = 'B';
                break;
            case 4:
            case 10:
            case 16:
                face = 'D';
                break;
            case 5:
            case 11:
            case 17:
                face = 'R';
                break;
        }
        if (*t >= 12) {
            fprintf(stdout, "2%c ", face);
        } else if (*t >= 6) {
            fprintf(stdout, "%c' ", face);
        } else {
            fprintf(stdout, "%c ", face);
        }
        ++t;
    }
}
