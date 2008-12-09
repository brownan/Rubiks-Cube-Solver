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
    /* The seven relevant corner cubies.  These are NOT null terminated */
    char cb1[6];
    char cb2[6];
    char cb3[6];
    char cb4[6];
    char cb5[6];
    char cb6[6];
    char cb7[6];
    
    
    char positions[] = {0,2,5,7,12,14,17,19};
    int index = 0;
    int pos, rot,num, i;

    memcpy(cb1, CUBIE(cubestr, 0), 6);
    memcpy(cb2, CUBIE(cubestr,2), 6);
    memcpy(cb3, CUBIE(cubestr,5), 6);
    memcpy(cb4, CUBIE(cubestr,7), 6);
    memcpy(cb5, CUBIE(cubestr,12), 6);
    memcpy(cb6, CUBIE(cubestr,14), 6);
    memcpy(cb7, CUBIE(cubestr,17), 6);
    
    /* First cubie */
    /* Now just looks at the stored cubie id of this cubie */
    num = *(CUBIE(cubestr,0) - 1);
#ifdef DEBUG_ASSERTS
    {
        int wp = whichpos(cb1);
        if (num != wp - 1) {
            fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
            fprintf(stderr, "whichpos: %d\n", wp);
            index = *((int *)0x0); /* sigsev */
        }
    }
#endif


    pos=0;
    /* Find its index in positions array.  In this case, it's
     * a number 0-7, for the next it's 0-6, and so fourth */
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    /* Remove that element from the array (that position is occupied
     * so no other cubie can be in it) */
    for (i=pos; i< 8-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    /* Munge the cb1 cubie string such that the first and second index
     * are not 'n', they are the first and second respective colors
     * that appear in the string */
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb1[i] != 'n') {
            cb1[0] = cb1[i];
            num++;
        } else if (num == 1 && cb1[i] != 'n') {
            cb1[1] = cb1[i];
            break;
        }
    }
    /* Identify the rotation of the cubie by finding which face has one of
     * the colors.  Blue in this case I believe is arbitrary, it could be
     * any color that appears in this cubie (if I remember correctly). */
    if (cb1[0] == 'b') rot = 0;
    else if (cb1[1] == 'b') rot = 1;
    else rot = 2;
    /* 3674160 = 21*18*15*12*9*6 */
    /* max(pos+rot*8) = 24 */
    /* so index after this line will be one one of the boundaries if the
     * hash space is split in 24 (24 places this cubie could go)
     * remember hash space is
     * 88179840 = 24*21*18*15*12*9*6
     */
    index += (pos + rot*8) * 3674160;
    /* The next cubie will add to the index such that the remaining hash
     * space (3674160) is split into 21 parts
     * and so fourth
     */
    
    /* second */
    num = *(CUBIE(cubestr,2) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb2) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 7-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb2[i] != 'n') {
            cb2[0] = cb2[i];
            num++;
        } else if (num == 1 && cb2[i] != 'n') {
            cb2[1] = cb2[i];
            break;
        }
    }
    if (cb2[0] == 'y') rot = 0;
    else if (cb2[1] == 'y') rot = 1;
    else rot = 2;
    index += (pos + rot*7) * 174960;
    
    /* third */
    num = *(CUBIE(cubestr,5) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb3) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 6-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb3[i] != 'n') {
            cb3[0] = cb3[i];
            num++;
        } else if (num == 1 && cb3[i] != 'n') {
            cb3[1] = cb3[i];
            break;
        }
    }
    if (cb3[0] == 'r') rot = 0;
    else if (cb3[1] == 'r') rot = 1;
    else rot = 2;
    index += (pos + rot*6) * 9720;

    /* fourth */
    num = *(CUBIE(cubestr,7) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb4) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 5-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb4[i] != 'n') {
            cb4[0] = cb4[i];
            num++;
        } else if (num == 1 && cb4[i] != 'n') {
            cb4[1] = cb4[i];
            break;
        }
    }
    if (cb4[0] == 'r') rot = 0;
    else if (cb4[1] == 'r') rot = 1;
    else rot = 2;
    index += (pos + rot*5) * 648;

    /* fifth */
    num = *(CUBIE(cubestr,12) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb5) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 4-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb5[i] != 'n') {
            cb5[0] = cb5[i];
            num++;
        } else if (num == 1 && cb5[i] != 'n') {
            cb5[1] = cb5[i];
            break;
        }
    }
    if (cb5[0] == 'w') rot = 0;
    else if (cb5[1] == 'w') rot = 1;
    else rot = 2;
    index += (pos + rot*4) * 54;

    /* sixth */
    num = *(CUBIE(cubestr,14) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb6) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 3-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb6[i] != 'n') {
            cb6[0] = cb6[i];
            num++;
        } else if (num == 1 && cb6[i] != 'n') {
            cb6[1] = cb6[i];
            break;
        }
    }
    if (cb6[0] == 'w') rot = 0;
    else if (cb6[1] == 'w') rot = 1;
    else rot = 2;
    index += (pos + rot*3) * 6;

    /* seventh */
    num = *(CUBIE(cubestr,17) - 1);
#ifdef DEBUG_ASSERTS
    if (num != whichpos(cb7) - 1) {
        fprintf(stderr, "\nWARNING: num != whichpos!!!\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb7[i] != 'n') {
            cb7[0] = cb7[i];
            num++;
        } else if (num == 1 && cb7[i] != 'n') {
            cb7[1] = cb7[i];
            break;
        }
    }
    if (cb7[0] == 'w') rot = 0;
    else if (cb7[1] == 'w') rot = 1;
    else rot = 2;
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
    stack = malloc(sizeof(stacktype));
    stack->rightblock = NULL; /* so it knows it's empty */

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
