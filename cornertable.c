#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cornertable.h"
#include "common.h"
#include "stack.h"
#include "cube.h"

/*
 * See more comments in cornertable.h
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
 * This implementation is pretty simple.
 * I'm thinking it may be better to just copy+paste this to wherever you
 * need it, maybe it'll save a stack call or something, or maybe it won't
 * matter
 */
char corner_lookup(const unsigned char *cornertable, const char *cubetohash)
{
    int hash = corner_hash(cubetohash);
    return hash&1 ? cornertable[(hash-1)/2] >> 4 : \
        cornertable[hash/2] & 15;
}

/* Returns 1 on success */
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

    /* create a temporary table to keep track of the stack */
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
            fprintf(stderr, "\r%d/88179840 hashed, on level:%d/11, total traversed:%d ", count, depth, popcount);
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
    return 1;
}

/*
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
