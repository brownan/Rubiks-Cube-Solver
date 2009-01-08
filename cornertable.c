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
 * corner_hash
 * Input: a cube_type string
 * Output: An integer in the range 0 to 88179840-1
 *
 * The general method is to process the first 7 of the 8 corner cubies.
 * For the first corner cube, since it has 24 possible positions, the
 * hash space is split into 24 even sections (3674160 elements long)
 * A number is calculated 0 through 23 that represents which position
 * the first corner cubie is in, and multiplied by 3674160, which becomes
 * the index.
 *
 * Then the second corner cube is similar, except the remaining space (which
 * is 3674160 elements long) is split into 21 equal parts, since the second
 * corner cubie has 21 possible positions.  That works out to 174960.  So,
 * similarly, a number 0-20 is calculated and multiplied by 174960 and then
 * added to the index.  This is repeated for each of the first 7 corner
 * cubies. The eighth cubie has only 1 possible position, so it is not
 * included in any calculations. (It cannot be rotated but one way since
 * the cube would not be solvable)
 *
 * This method takes about 37 mins on my 2GHz Intel T7200
 */
int corner_hash(const char *cubestr)
{
    char positions[] = {0,2,5,7,12,14,17,19};
    int index = 0;
    int pos, rot,num, i;

    char *cubie; /* points to the current cubie in question */

    /* First cubie */
    cubie = CUBIE(cubestr, 0);
    num = cubie[0]; /* position id */
    pos=0;
    /* Find its index in positions array.  In this case, it's
     * a number 0-7, for the next it's 0-6, and so fourth */
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    /* Remove that element from the array (that position is occupied
     * so no other cubie can be in it) */
    for (i=pos; i< 7; i++)
        positions[i] = positions[i+1];
    /* 3674160 = 21*18*15*12*9*6 */
    /* pos+rot*8 has range 0 - 23 */
    /* so index after this line will be one one of the boundaries if the
     * hash space is split in 24 (24 places this cubie could go)
     * remember total hash space is
     * 88179840 = 24*21*18*15*12*9*6
     */
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*8) * 3674160;
    /* The next cubie will add to the index such that the remaining hash space
     * (3674160) is split into 21 parts and so fourth (hence taking the pos out
     * of the positions array, now pos can only be 0-6)
     */
    
    /* second */
    cubie = CUBIE(cubestr, 2);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 6; i++)
        positions[i] = positions[i+1];
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*7) * 174960;
    
    /* third */
    cubie = CUBIE(cubestr, 5);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 5; i++)
        positions[i] = positions[i+1];
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*6) * 9720;

    /* fourth */
    cubie = CUBIE(cubestr, 7);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 4; i++)
        positions[i] = positions[i+1];
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*5) * 648;

    /* fifth */
    cubie = CUBIE(cubestr, 12);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 3; i++)
        positions[i] = positions[i+1];
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*4) * 54;

    /* sixth */
    cubie = CUBIE(cubestr, 14);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 2; i++)
        positions[i] = positions[i+1];
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*3) * 6;

    /* seventh */
    cubie = CUBIE(cubestr, 17);
    num = cubie[0]; /* position id */
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    rot = cubie[1]; /* 0 - 2 */
    index += (pos + rot*2);
    
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
            hash = corner_hash(current.cube_data);
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
                hash = corner_hash(turned);
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
