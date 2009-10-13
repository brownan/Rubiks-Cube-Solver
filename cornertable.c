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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cornertable.h"
#include "common.h"
#include "stack.h"
#include "cube.h"

/*
 * cornertable.c
 * this has functions to generate the 88179840 element corner heuristics table,
 * as well as methods to use it
 */

/*
 * corner_map
 * Input: a cube_type string
 * Output: An integer in the range 0 to 88179840-1
 *
 * How this works:
 * First find which permutation the 8 corner cubies are in with respect
 * to their position. There are 8! permutations, and are mapped
 * to a number 0 through 8!-1.
 * To do this, first the inversion vector is computed. Then, the vector
 * is treated as a factoradic number and is converted into decimal.
 *
 * The rotations also need to be factored in. There are 3^7 rotation
 * configurations. Since each rotation is independent (discarding the last
 * cubie which is determined by the other 7) I treat the rotations as a 7 digit
 * number in base 3. It's converted to decimal to get a number 0 though 3^7-1
 *
 * These two numbers are then combined thusly:
 * permutation * 3^7 + rotation
 */
int corner_map(const char *cubestr)
{
    int index;

    int inversions[8] = {0,0,0,0,0,0,0,0};
    int positions[8];
    int i, j;

    // Put all the positions into the positions array
    positions[0] = CUBIE(cubestr,0)[0];
    positions[1] = CUBIE(cubestr,2)[0];
    positions[2] = CUBIE(cubestr,5)[0];
    positions[3] = CUBIE(cubestr,7)[0];
    positions[4] = CUBIE(cubestr,12)[0];
    positions[5] = CUBIE(cubestr,14)[0];
    positions[6] = CUBIE(cubestr,17)[0];
    positions[7] = CUBIE(cubestr,19)[0];

    for (i=0; i<8; i++)
        for (j=i+1; j<8; j++)
            if (positions[i] > positions[j])
                inversions[i]++;

    // the inversion vector is computed. Now compute the permutation index
    // using factoradic numbering
    index =  inversions[0] * 7*6*5*4*3*2*1;
    index += inversions[1] * 6*5*4*3*2*1;
    index += inversions[2] * 5*4*3*2*1;
    index += inversions[3] * 4*3*2*1;
    index += inversions[4] * 3*2*1;
    index += inversions[5] * 2*1;
    index += inversions[6] * 1;
    index += inversions[7]; // * 0!

    index *= 2187; // (3^7)

    // Now compute the rotations and add them into the index
    // treat each rotation value as a digit in base 3
    // skip the last one
    index += CUBIE(cubestr,0)[1] * 3*3*3*3*3*3;
    index += CUBIE(cubestr,2)[1] * 3*3*3*3*3;
    index += CUBIE(cubestr,5)[1] * 3*3*3*3;
    index += CUBIE(cubestr,7)[1] * 3*3*3;
    index += CUBIE(cubestr,12)[1]* 3*3;
    index += CUBIE(cubestr,14)[1]* 3;
    index += CUBIE(cubestr,17)[1];


#ifdef DEBUG_ASSERTS
    if (index >= 88179840) {
        fprintf(stderr, "\nWARNING: HASH RETURNED %d\n", index);
        index = *((int *)0x0); /* sigsev */
    }
#endif
    return index;

}

/*
 * Generates the corner heuristics table about the given solution, which could
 * be any valid cube.  The table is stored in the given character pointer,
 * which should be allocated with the CORNER_TABLE_NEW macro
 * return 1 on success
 */
int corner_generate(unsigned char *cornertable, const char *solution)
{
    stacktype *stack;
    /*
     * qdata is a struct, holding cube string 'cube_data',
     * an int representing the turn that was made, 'turn',
     * and an int representing the distance, 'distance'
     */
    qdata current;
    int count = 0; /* total hashed */
    int popcount = 0; /* total traversed */
    int hash;
    int i;
    unsigned char *instack;
    int depth;
    cube_type turned;


    /* Create a stack */
    stack = STACK_NEW;

    /*
     * create a temporary table to keep track of the stack This holds the value
     * of each item that has been added to the stack, and its distance.  This
     * way, we know if we encounter the same node but at a further distance we
     * can throw it away.  This heuristic cuts down on processing time by quite
     * a bit, and is only as time intensive as the hashing algorithm, at the
     * expense of using more memory.
     */
    instack = CORNER_TABLE_NEW;

    depth = -1;
    while (count < 88179840)
    {
        /* if stack is empty, go up a level */
        if (stack->length == 0)
        {
            stack_push(stack, solution, -1, 0);
            depth++;
            /* clear out instack table */
            CORNER_TABLE_CLEAR(instack);
        }

        /* Pop the first item off, put it in current */
        stack_peek_cube(stack, &(current.cube_data));
        current.turn = stack_peek_turn(stack);
        current.distance = stack_peek_distance(stack);
        stack_pop(stack);
        popcount++;

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popcount & 0777777) == 0777777) {
            fprintf(stderr, "\r%d/88179840 hashed, on level:%d/11, total traversed:%d %d%% ", count, depth, popcount, popcount/5788130);
        }
#ifdef PROFILE_MODE
        /* For profiling, so I don't have to wait an hour to gather data */
        if (count == 10000000) {
            return 0;
        }
#endif

        /*
         * if item is at our current target depth, add it to hash table
         */
        if (current.distance == depth) {
            hash = corner_map(current.cube_data);
            if (hash & 1) {
                if (!(cornertable[(hash-1)/2] >> 4)) {
                    cornertable[(hash-1)/2] |= current.distance << 4;
                    count++;
                } else {
                    /* A duplicate, skip */
                }
            } else {
                if (!(cornertable[hash/2]&15)) {
                    cornertable[hash/2] |= current.distance;
                    count++;
                } else {
                    /* a duplicate */
                }
            }
        } else {
            /* Not at the current depth, put all turns onto the stack */
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
                hash = corner_map(turned);
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

                /* Add to real stack */
                stack_push(stack, turned, i, current.distance+1);
            }
        }

    }
    free(instack);
    while(stack->length) {
        stack_pop(stack);
    }
    free(stack);
    fprintf(stderr, "\n");
    return 1;
}

/*
 * These methods read and write the given table to/from the given
 * FILE handle
 * Return 1 on success 0 on failure
 */
int corner_write(unsigned char *cornertable, FILE *output)
{
    int written;
    written = fwrite(cornertable, 1, 44089920, output);
    if (written < 44089920)
        return 0;
    return 1;
}
int corner_read(unsigned char *cornertable, FILE *input)
{
    int read;
    read = fread(cornertable, 1, 44089920, input);
    if (read < 44089920) {
        return 0;
    }
    return 1;
}
