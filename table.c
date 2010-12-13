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

    Copyright © 2010 Andrew Brown <brownan@gmail.com>
*/

#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "stack.h"
#include "edgetable.h"
#include "cornertable.h"

static int generate(unsigned char *table, struct heuristic heuristics_info);
static int write(unsigned char *table, FILE *output, int length);
static int read(unsigned char *table, FILE *input, int length);

/*
 * This file has general routines to generate a table. The tables store 4 bit
 * values, and part of a table definition is a function that takes the cube
 * configuration state and returns an index into the table.
 */

/**
 * table_generate takes a pointer to a heuristic struct, and generates its
 * table.
 */
unsigned char *table_generate(struct heuristic *heuristic_info)
{
    FILE *output;
    unsigned char *rtable = calloc(heuristic_info->num_entries>>1,1);

    // De-reference the heuristic struct for this call
    fprintf(stderr, "Generating table now, will save to %s when done\n", heuristic_info->filename);
    generate(rtable, *heuristic_info);

    output = fopen(heuristic_info->filename, "w");
    write(rtable, output, heuristic_info->num_entries>>1);
    fclose(output);
    fprintf(stderr, "Done. Saved to %s\n", heuristic_info->filename);

    return rtable;
}

unsigned char *table_load(struct heuristic *heuristic_info)
{
    FILE *input = fopen(heuristic_info->filename, "r");
    unsigned char *rtable = calloc(heuristic_info->num_entries>>1,1);

    read(rtable, input, heuristic_info->num_entries>>1);

    return rtable;
}

/*
 * Writes a table out
 */
static int write(unsigned char *table, FILE *output, int length)
{
    int written;
    written = fwrite(table, 1, length, output);
    if (written < length)
        return 0;
    return 1;
}

/*
 * reads in a table
 */
static int read(unsigned char *table, FILE *input, int length)
{
    int read;
    read = fread(table, 1, length, input);
    if (read < length) {
        return 0;
    }
    return 1;
}

/*
 * This actually does the generating.
 * Takes the heuristic struct right on the stack, not a pointer
 */
static int generate(unsigned char *table, struct heuristic heuristics_info)
{

    stacktype *stack;
    /*
     * qdata is a struct, holding cube string 'cube_data',
     * an int representing the turn that was made, 'turn',
     * and an int representing the distance, 'distance'
     */
    qdata current;

    /*
     * total entries of the table filled
     * this starts at 1 because we account for the "solved" cube specially,
     * since that has a distance 0
     */
    int count = 1;
    int popcount = 0; /* total traversed */
    int hash;
    int i;
    unsigned char *instack;
    int depth;
    cube_type turned;

    int solvedhash = heuristics_info.map_function(cube_solved);

    /* Create a stack */
    stack = STACK_NEW;

    /*
     * create a temporary table to keep track of the stack. This holds the value
     * of each item that has been added to the stack, and its distance, for
     * the current depth.  This way, we know if we encounter the same node but
     * at a further distance we can throw it away.  This heuristic cuts down on
     * processing time by quite a bit, and is only as time intensive as the
     * hashing algorithm, at the expense of using more memory.
     */
    instack = calloc(heuristics_info.num_entries>>1,1);

    depth = 0;
    while (count < heuristics_info.num_entries)
    {
        /* if stack is empty, go up a level */
        if (stack->length == 0)
        {
            stack_push(stack, cube_solved, -1, 0);
            depth++;
            /* clear out instack table */
            memset(instack, 255, heuristics_info.num_entries>>1);
            /* set solved state to 0, so it's not traversed at all. This is
             * necessary for some tables but not for others because of how the
             * traversal differs. For tables where every possible move affects
             * the state, the zero state won't be visited, but for others, it
             * may be. To make the algorithm consistent, the solved state is
             * marked so it won't be visited at all.
             * This also means that instack is not just an optimization, but is
             * required for correctness.
             */
            if (solvedhash & 1) {
                /* zero out upper bits */
                instack[(solvedhash-1)/2] &= 0x0f;
            } else {
                /* zero out lower bits */
                instack[solvedhash/2] &= 0xf0;
            }
        }

        /* Pop the first item off, put it in current */
        stack_peek_cube(stack, &(current.cube_data));
        current.turn = stack_peek_turn(stack);
        current.distance = stack_peek_distance(stack);
        stack_pop(stack);
        popcount++;

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popcount & 0777777) == 0777777) {
            fprintf(stderr, "\r%d/%d hashed, on level:%d/%d, total traversed:%d",
                   count, heuristics_info.num_entries,
                   depth, heuristics_info.max_level,
                   popcount);
        }
#ifdef PROFILE_MODE
        /* For profiling, so I don't have to wait an hour to gather data */
        if (count == 10000000) {
            return 0;
        }
#endif

        /* Look at all turns of the current cube */
        for (i=0; i<18; i++) {
            /* Determine if we should skip this turn */
            if (current.turn != -1 && SHOULDIAVOID(i, current.turn)) {
                continue;
            }

            memcpy(turned, current.cube_data, CUBELEN);
            cube_turn(turned, i);

            /*
             * Check if turned is in instack and is greater than
             * or equal to the depth.  If so, skip
             */
            hash = heuristics_info.map_function(turned);
            if (hash&1 ? \
                    ((instack[(hash-1)/2] >> 4) <= (current.distance+1)) : \
                    ((instack[hash/2] & 15) <= (current.distance+1))) {
                continue;
            }
            /* add to instack */
            if (hash&1) {
                instack[(hash-1)/2] &= 15;
                instack[(hash-1)/2] |= (current.distance+1) << 4;
            } else {
                instack[hash/2] &= 15<<4;
                instack[hash/2] |= (current.distance+1);
            }

            if (current.distance+1 == depth) {
                /*
                 * if item is at our current target depth, add it to the table
                 */

                if (hash & 1) {
                    if (!(table[(hash-1)/2] >> 4)) {
                        table[(hash-1)/2] |= (current.distance+1) << 4;
                        count++;
                    } else {
                        /* A duplicate, skip */
                    }
                } else {
                    if (!(table[hash/2]&15)) {
                        table[hash/2] |= current.distance + 1;
                        count++;
                    } else {
                        /* a duplicate */
                    }
                }
            } else {
                /* Add to real stack */
                stack_push(stack, turned, i, current.distance+1);
            }
        }

    }

    /*
     * Zero out the solved slot, which doesn't get computed correctly by the
     * above algorithm
     */
    hash = heuristics_info.map_function(cube_solved);
    if (hash & 1) {
        /* zero out upper bits */
        table[(hash-1)/2] &= 15;
    } else {
        /* zero out lower bits */
        table[hash/2] &= (15<<4);
    }

    free(instack);
    while(stack->length) {
        stack_pop(stack);
    }
    free(stack);
    fprintf(stderr, "\n");
    return 1;
}

/**
 * Table definitions
 */
struct heuristic edge_table1 = {
    edge_hash1,
    42577920,
    10,
    "new_edgetable.rht"
};

struct heuristic corner_table = {
    corner_map,
    88179840,
    11,
    "new_cornertable.rht"
};

int main()
{
    table_generate(&corner_table);
    return 0;
}
